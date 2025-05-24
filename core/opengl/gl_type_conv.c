#include "gl_type_conv.h"

#include <glad/glad.h>

uint32_t buf_obj_type_to_gl(buf_obj_type_t type)
{
  switch (type) {
    case buf_arr: return GL_ARRAY_BUFFER;
    case buf_idx: return GL_ELEMENT_ARRAY_BUFFER;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t draw_mode_to_gl(draw_mode_t mode)
{
  switch (mode) {
    case draw_static: return GL_STATIC_DRAW;
    case draw_dynamic: return GL_DYNAMIC_DRAW;
    case draw_stream: return GL_STREAM_DRAW;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t data_type_to_gl(data_type_t type)
{
  switch (type) {
    case type_unknown: return GL_BYTE;
    case type_half: return GL_HALF_FLOAT;
    case type_float: return GL_FLOAT;
    case type_double: return GL_DOUBLE;
    case type_uchar: return GL_UNSIGNED_BYTE;
    case type_char: return GL_BYTE;
    case type_ushort: return GL_UNSIGNED_SHORT;
    case type_short: return GL_SHORT;
    case type_uint: return GL_UNSIGNED_INT;
    case type_int: return GL_INT;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t idx_mode_to_gl(idx_mode_t mode)
{
  switch (mode) {
    case idx_triangles: return GL_TRIANGLES;
    case idx_triangle_strip: return GL_TRIANGLE_STRIP;
    case idx_triangle_fan: return GL_TRIANGLE_FAN;
    case idx_lines: return GL_LINES;
    case idx_line_strip: return GL_LINE_STRIP;
    case idx_line_loop: return GL_LINE_LOOP;
    case idx_points: return GL_POINTS;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t img_fmt_to_gl(img_fmt_t format)
{
  switch (format) {
    case img_fmt_r8: return GL_RED;
    case img_fmt_ra8: return GL_RG;
    case img_fmt_rgb8: return GL_RGB;
    case img_fmt_rgba8: return GL_RGBA;
    case img_fmt_invalid: return GL_RGBA;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t tex_filter_to_gl(tex_filter_t filter)
{
  switch (filter) {
    case tex_filter_nearest: return GL_NEAREST;
    case tex_filter_nearest_mipmap: return GL_NEAREST_MIPMAP_LINEAR;
    case tex_filter_nearest_mipmap_nearest:
      return GL_NEAREST_MIPMAP_NEAREST;
    case tex_filter_linear: return GL_LINEAR;
    case tex_filter_linear_mipmap: return GL_LINEAR_MIPMAP_LINEAR;
    case tex_filter_linear_mipmap_nearest: return GL_LINEAR_MIPMAP_NEAREST;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t tex_wrap_to_gl(tex_wrap_t wrap)
{
  switch (wrap) {
    case tex_wrap_rep: return GL_REPEAT;
    case tex_wrap_mirror_rep: return GL_MIRRORED_REPEAT;
    case tex_wrap_clamp_edge: return GL_CLAMP_TO_EDGE;
    case tex_wrap_clamp_border: return GL_CLAMP_TO_BORDER;
  }
  return GL_INVALID_ENUM; // unreachable
}
