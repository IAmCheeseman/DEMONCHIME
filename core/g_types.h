#ifndef __engine_gfx_gfx_types__
#define __engine_gfx_gfx_types__

#include "include.h"

typedef enum shader
{
  shader_none = -1,
  shader_default,
  shader_billboard,
  shader_2d,
  shader_text,
  shader_count,
} shader_t;

typedef enum gfx_backend_type
{
  gfx_backend_opengl,
} gfx_backend_type_t;

typedef enum buf_obj_type
{
  buf_arr,
  buf_idx,
} buf_obj_type_t;

typedef enum draw_mode
{
  draw_static,
  draw_dynamic,
  draw_stream,
} draw_mode_t;

typedef enum idx_mode
{
  idx_triangles,
  idx_triangle_strip,
  idx_triangle_fan,
  idx_lines,
  idx_line_strip,
  idx_line_loop,
  idx_points,
} idx_mode_t;

typedef enum data_type
{
  type_unknown = 0,

  type_half,
  type_float,
  type_double,

  type_uchar,
  type_char,

  type_ushort,
  type_short,

  type_uint,
  type_int,
} data_type_t;

typedef enum img_fmt
{
  img_fmt_invalid = 0,
  img_fmt_r8,
  img_fmt_ra8,
  img_fmt_rgb8,
  img_fmt_rgba8,
} img_fmt_t;

typedef enum tex_filter
{
  tex_filter_nearest,
  tex_filter_nearest_mipmap,
  tex_filter_nearest_mipmap_nearest,
  tex_filter_linear,
  tex_filter_linear_mipmap,
  tex_filter_linear_mipmap_nearest,
} tex_filter_t;

typedef enum tex_wrap
{
  tex_wrap_rep,
  tex_wrap_mirror_rep,
  tex_wrap_clamp_edge,
  tex_wrap_clamp_border,
} tex_wrap_t;

#define framebuffer_color_buf (1 << 0)
#define framebuffer_depth_buf (1 << 1)
#define framebuffer_depth_mask_buf (1 << 2)
#define framebuffer_drawable (1 << 3)

size_t get_data_type_size(data_type_t type);
data_type_t str_to_data_type(const char* str, size_t len);

#endif
