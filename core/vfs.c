// based on https://github.com/rxi/juno/blob/master/src/fs.c

#include "vfs.h"

#include <stdio.h>
#include <sys/stat.h>

#include "mem.h"

#define fseeko fseek
#define ftello ftell
#define _fseeki64 fseek
#define _ftelli64 ftell
#include "miniz/miniz.c"

// Design goals:
// - can initialize any number of virtual filesystems
// - using a filesystem initialized from a zip is the same as one made from 
//   directories, with the exception of writing being disabled.
// - any given file paths should work fine

// to concat any file path to another:
// - make sure the rhs path does not have a cwd prefix (./)

static bool is_char_path_sep(char c)
{
  return c == '/' || c == '\\';
}

// removes ./ from the beginning of paths
static const char* rem_cwd_prefix(const char* path)
{
  if (path[0] == '.' && is_char_path_sep(path[1])) return path + 2;
  return path;
}

// checks if a path is a dir
static bool is_path_dir(const char* path)
{
  struct stat f;
  int res = stat(path, &f);
  return res == 0 && S_ISDIR(f.st_mode);
}

static bool is_path_valid(const char *filename) {
  return !(
    *filename == '/' ||
    strstr(filename, "..") ||
    strstr(filename, ":\\"));
}

vfs_error_t vfs_mount(vfs_t** vfs, const char* path)
{
  size_t len = strlen(path);
  char* owned_path = (char*)mem_alloc(sizeof(char) * (len + 1));
  strcpy(owned_path, path);
  if (is_char_path_sep(owned_path[len - 1])) owned_path[len - 1] = '\0';

  vfs_t* mnt = (vfs_t*)mem_alloc(sizeof(vfs_t));

  bool is_dir = is_path_dir(owned_path);
  memset(&mnt->zip, 0, sizeof(mz_zip_archive));
  if (!is_dir && !mz_zip_reader_init_file(&mnt->zip, owned_path, 0)) {
    mem_destroy(mnt);
    mem_destroy(owned_path);
    return VFS_COULD_NOT_MOUNT;
  }

  mnt->path = owned_path;
  mnt->type = is_dir ? VFS_DIR : VFS_ZIP;
  mnt->next = *vfs;
  *vfs = mnt;

  log_info("mounted vfs at '%s'", path);
  
  return VFS_OK;
}

static bool does_file_exist_in_mount(vfs_t* vfs, const char* path)
{
  if (vfs->type == VFS_DIR) {
    struct stat stats;
    char* full_path = concat(vfs->path, "/", path, NULL);
    if (!full_path) return false;
    int res = stat(full_path, &stats);
    mem_destroy(full_path);
    return res == 0;
  } else if (vfs->type == VFS_ZIP) {
    int index = mz_zip_reader_locate_file(&vfs->zip, path, NULL, 0);
    return index != -1;
  }
  return false;
}

bool vfs_exists(vfs_t* vfs, const char* path)
{
  if (!is_path_valid(path)) return false;
  path = rem_cwd_prefix(path);

  vfs_t* mnt = vfs;

  while (mnt) {
    if (does_file_exist_in_mount(mnt, path)) return true;
    mnt = mnt->next;
  }
  return false;
}

char* vfs_read(vfs_t* vfs, const char* path, size_t* size)
{
  if (!is_path_valid(path)) {
    log_warning("could not load '%s', because the path is invalid", path);
    return NULL;
  }
  const char* lpath = rem_cwd_prefix(path);

  vfs_t* mnt = vfs;

  while (mnt) {
    if (!does_file_exist_in_mount(mnt, lpath)) {
      mnt = mnt->next; // skip
      continue;
    }

    if (mnt->type == VFS_DIR) {
      char* full_path = concat(mnt->path, "/", lpath, NULL);
      FILE* file = fopen(full_path, "rb");
      if (file == NULL) {
        log_warning("could not open file '%s'", full_path);
        mem_destroy(full_path);
        return NULL;
      }

      fseek(file, 0L, SEEK_END);
      size_t file_size = ftell(file);
      rewind(file);

      char* dat = (char*)mem_alloc(sizeof(char) * (file_size + 1));

      size_t bytes_read = fread(dat, sizeof(char), file_size, file);
      if (bytes_read < file_size) {
        log_warning("could not read file '%s'", full_path);
        mem_destroy(dat);
        mem_destroy(full_path);
        return NULL;
      }
      fclose(file);

      if (size) *size = file_size;

      log_debug("loaded file '%s'", full_path);
      mem_destroy(full_path);
      return dat;
    } else if (mnt->type == VFS_ZIP) {
      size_t zdat_size;
      char* zdat = mz_zip_reader_extract_file_to_heap(
        &mnt->zip, lpath, &zdat_size, 0);
      if (!zdat) {
        log_warning("could not load file '%s'", path);
        return NULL;
      }

      // miniz doesn't null terminate their stuff :(

      char* dat = (char*)mem_alloc(sizeof(char) * (zdat_size + 1));
      memcpy(dat, zdat, zdat_size);

      if (size) *size = zdat_size;

      free(zdat);

      log_debug("loaded file '%s'", path);
      return dat;
    }
    mnt = mnt->next;
  }

  log_warning("could not find file '%s'", path);
  return NULL;
}

char* vfs_read_txt(vfs_t* vfs, const char* path, size_t* size)
{
  size_t size_;
  char* dat = vfs_read(vfs, path, &size_);
  if (!dat) return dat;
  if (size) *size = size_;
  dat[size_] = '\0';
  return dat;
}

void vfs_destroy(vfs_t* vfs)
{
  vfs_t* mnt = vfs;

  while (mnt) {
    vfs_t* next = mnt->next;
    if (mnt->type == VFS_ZIP) mz_zip_reader_end(&mnt->zip);
    mem_destroy(mnt->path);
    mem_destroy(mnt);
    mnt = next;
  }
}
