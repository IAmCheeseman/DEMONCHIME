#ifndef __engine_vfs__
#define __engine_vfs__

#include "include.h"

#include "miniz.h"

typedef enum vfs_err
{
  vfs_ok = 0,
  vfs_cannot_mnt,
  vfs_file_not_found,
  vfs_no_mem,
  vfs_malformed_path,
  vfs_cannot_open,
  vfs_cannot_write,
  vfs_cannot_read,
} vfs_err_t;

typedef enum vfs_type
{
  vfs_dir,
  vfs_zip,
} vfs_type_t;

typedef struct vfs
{
  struct vfs* next;
  vfs_type_t type;
  mz_zip_archive zip;
  char* path;
} vfs_t;

vfs_err_t vfs_mount(vfs_t** vfs, const char* path);
void vfs_destroy(vfs_t* vfs);
bool vfs_exists(vfs_t* vfs, const char* path);
// read the entirity of a file
uint8_t* vfs_read(vfs_t* vfs, const char* path, size_t* size);
// same as `vfs_read`, except it adds a null terminator
char* vfs_read_txt(vfs_t* vfs, const char* path, size_t* size);

#endif
