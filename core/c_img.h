#ifndef __engine_image__
#define __engine_image__

#include "include.h"
#include "g_types.h"
#include "m_math.h"
#include "c_vfs.h"

typedef struct img
{
  uint8_t* data;
  vec2i_t size;
  img_fmt_t fmt;
} img_t;

img_t image_load(vfs_t* vfs, const char* path);
// will take ownership of data
img_t image_load_from_mem(uint8_t* data, vec2i_t size, img_fmt_t format);
void image_destroy(img_t* img);

#endif
