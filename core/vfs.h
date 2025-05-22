#ifndef __engine_vfs__
#define __engine_vfs__

#include "include.h"

#include "miniz.h"

typedef enum vfs_err_e
{
  VFS_OK = 0,
  VFS_COULD_NOT_MOUNT,
  VFS_FILE_NOT_FOUND,
  VFS_OUT_OF_MEM,
  VFS_MALFORMED_PATH,
  VFS_CANNOT_OPEN,
  VFS_CANNOT_WRITE,
  VFS_CANNOT_READ,
} vfs_err_t;

typedef enum vfs_type_e
{
  VFS_DIR,
  VFS_ZIP,
} vfs_type_t;

typedef struct vfs_s
{
  struct vfs_s* next;
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
