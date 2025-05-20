#ifndef __engine_gfx_gfx_types__
#define __engine_gfx_gfx_types__

#include "include.h"

typedef enum buf_obj_type_e
{
  BUFFER_ARRAY,
  BUFFER_INDEX,
} buf_obj_type_t;

typedef enum draw_mode_e
{
  DRAW_STATIC,
  DRAW_DYNAMIC,
  DRAW_STREAM,
} draw_mode_t;

typedef enum idx_mode_e
{
  INDEX_TRIANGLES,
  INDEX_TRIANGLE_STRIP,
  INDEX_TRIANGLE_FAN,
  INDEX_LINES,
  INDEX_LINE_STRIP,
  INDEX_LINE_LOOP,
  INDEX_POINTS,
} idx_mode_t;

typedef enum data_type_e
{
  TYPE_UNKNOWN = 0,

  TYPE_HALF,
  TYPE_FLOAT,
  TYPE_DOUBLE,

  TYPE_UCHAR,
  TYPE_CHAR,

  TYPE_USHORT,
  TYPE_SHORT,

  TYPE_UINT,
  TYPE_INT,
} data_type_t;

typedef enum img_fmt_e
{
  IMAGE_FORMAT_INVALID = 0,
  IMAGE_FORMAT_R8,
  IMAGE_FORMAT_RA8,
  IMAGE_FORMAT_RGB8,
  IMAGE_FORMAT_RGBA8,
} img_fmt_t;

typedef enum tex_filter_e
{
  TEXTURE_FILTER_NEAREST,
  TEXTURE_FILTER_NEAREST_MIPMAP,
  TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST,
  TEXTURE_FILTER_LINEAR,
  TEXTURE_FILTER_LINEAR_MIPMAP,
  TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST,
} tex_filter_t;

typedef enum tex_wrap_e
{
  TEXTURE_WRAP_REPEAT,
  TEXTURE_WRAP_MIRRORED_REPEAT,
  TEXTURE_WRAP_CLAMP_EDGE,
  TEXTURE_WRAP_CLAMP_BORDER,
} tex_wrap_t;

#define FRAMEBUFFER_COLOR_BUF (1 << 0)
#define FRAMEBUFFER_DEPTH_BUF (1 << 1)
#define FRAMEBUFFER_DEPTH_MASK_BUF (1 << 2)
#define FRAMEBUFFER_DRAWABLE (1 << 3)

size_t get_data_type_size(data_type_t type);
data_type_t str_to_data_type(const char* str, size_t len);

#endif
