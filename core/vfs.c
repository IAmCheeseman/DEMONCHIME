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

static bool IsCharPathSep(char c)
{
  return c == '/' || c == '\\';
}

// removes ./ from the beginning of paths
static const char* RemoveCwdPrefix(const char* path)
{
  if (path[0] == '.' && IsCharPathSep(path[1])) return path + 2;
  return path;
}

// checks if a path is a dir
static bool IsPathDir(const char* path)
{
  struct stat f;
  int res = stat(path, &f);
  return res == 0 && S_ISDIR(f.st_mode);
}

static bool IsFilenameValid(const char *filename) {
  return !(
    *filename == '/' ||
    strstr(filename, "..") ||
    strstr(filename, ":\\"));
}

enum VfsError VfsMount(struct Vfs** vfs, const char* path)
{
  size_t len = strlen(path);
  char* owned_path = (char*)Alloc(sizeof(char) * (len + 1));
  strcpy(owned_path, path);
  if (IsCharPathSep(owned_path[len - 1])) owned_path[len - 1] = '\0';

    struct Vfs* mnt = (struct Vfs*)Alloc(sizeof(struct Vfs));

  bool is_dir = IsPathDir(owned_path);
  memset(&mnt->zip, 0, sizeof(mz_zip_archive));
  if (!is_dir && !mz_zip_reader_init_file(&mnt->zip, owned_path, 0)) {
    Destroy(mnt);
    Destroy(owned_path);
    return VFS_COULD_NOT_MOUNT;
  }

  mnt->path = owned_path;
  mnt->type = is_dir ? VFS_DIR : VFS_ZIP;
  mnt->next = *vfs;
  *vfs = mnt;

  LogInfo("mounted vfs at '%s'", path);
  
  return VFS_OK;
}

static bool DoesFileExistInMount(struct Vfs* vfs, const char* path)
{
  if (vfs->type == VFS_DIR) {
    struct stat stats;
    char* full_path = Concat(vfs->path, "/", path, NULL);
    if (!full_path) return false;
    int res = stat(full_path, &stats);
    Destroy(full_path);
    return res == 0;
  } else if (vfs->type == VFS_ZIP) {
    int index = mz_zip_reader_locate_file(&vfs->zip, path, NULL, 0);
    return index != -1;
  }
  return false;
}

bool VfsDoesFileExist(struct Vfs* vfs, const char* path)
{
  if (!IsFilenameValid(path)) return false;
  path = RemoveCwdPrefix(path);

  struct Vfs* mnt = vfs;

  while (mnt) {
    if (DoesFileExistInMount(mnt, path)) return true;
    mnt = mnt->next;
  }
  return false;
}

char* VfsReadFile(struct Vfs* vfs, const char* path, size_t* size)
{
  if (!IsFilenameValid(path)) {
    LogWarning("could not load '%s', because the path is invalid", path);
    return NULL;
  }
  const char* lpath = RemoveCwdPrefix(path);

  struct Vfs* mnt = vfs;

  while (mnt) {
    if (!DoesFileExistInMount(mnt, lpath)) {
      mnt = mnt->next; // skip
      continue;
    }

    if (mnt->type == VFS_DIR) {
      char* full_path = Concat(mnt->path, "/", lpath, NULL);
      FILE* file = fopen(full_path, "rb");
      if (file == NULL) {
        LogWarning("could not open file '%s'", full_path);
        Destroy(full_path);
        return NULL;
      }

      fseek(file, 0L, SEEK_END);
      size_t file_size = ftell(file);
      rewind(file);

      char* dat = (char*)Alloc(sizeof(char) * (file_size + 1));

      size_t bytes_read = fread(dat, sizeof(char), file_size, file);
      if (bytes_read < file_size) {
        LogWarning("could not read file '%s'", full_path);
        Destroy(dat);
        Destroy(full_path);
        return NULL;
      }
      fclose(file);

      if (size) *size = file_size;

      LogDebug("loaded file '%s'", full_path);
      Destroy(full_path);
      return dat;
    } else if (mnt->type == VFS_ZIP) {
      size_t zdat_size;
      char* zdat = mz_zip_reader_extract_file_to_heap(
        &mnt->zip, lpath, &zdat_size, 0);
      if (!zdat) {
        LogWarning("could not load file '%s'", path);
        return NULL;
      }

      // miniz doesn't null terminate their stuff :(

      char* dat = (char*)Alloc(sizeof(char) * (zdat_size + 1));
      memcpy(dat, zdat, zdat_size);

      if (size) *size = zdat_size;

      free(zdat);

      LogDebug("loaded file '%s'", path);
      return dat;
    }
    mnt = mnt->next;
  }

  LogWarning("could not find file '%s'", path);
  return NULL;
}

char* VfsReadTxtFile(struct Vfs* vfs, const char* path, size_t* size)
{
  size_t size_;
  char* dat = VfsReadFile(vfs, path, &size_);
  if (!dat) return dat;
  if (size) *size = size_;
  dat[size_] = '\0';
  return dat;
}

void VfsDestroy(struct Vfs* vfs)
{
  struct Vfs* mnt = vfs;

  while (mnt) {
    struct Vfs* next = mnt->next;
    if (mnt->type == VFS_ZIP) mz_zip_reader_end(&mnt->zip);
    Destroy(mnt->path);
    Destroy(mnt);
    mnt = next;
  }
}
