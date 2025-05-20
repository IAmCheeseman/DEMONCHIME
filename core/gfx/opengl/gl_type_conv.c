#include "gl_type_conv.h"

#include <glad/glad.h>

uint32_t buf_obj_type_to_gl(buf_obj_type_t type)
{
  switch (type) {
    case BUFFER_ARRAY: return GL_ARRAY_BUFFER;
    case BUFFER_INDEX: return GL_ELEMENT_ARRAY_BUFFER;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t draw_mode_to_gl(draw_mode_t mode)
{
  switch (mode) {
    case DRAW_STATIC: return GL_STATIC_DRAW;
    case DRAW_DYNAMIC: return GL_DYNAMIC_DRAW;
    case DRAW_STREAM: return GL_STREAM_DRAW;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t data_type_to_gl(data_type_t type)
{
  switch (type) {
    case TYPE_UNKNOWN: return GL_BYTE;
    case TYPE_HALF: return GL_HALF_FLOAT;
    case TYPE_FLOAT: return GL_FLOAT;
    case TYPE_DOUBLE: return GL_DOUBLE;
    case TYPE_UCHAR: return GL_UNSIGNED_BYTE;
    case TYPE_CHAR: return GL_BYTE;
    case TYPE_USHORT: return GL_UNSIGNED_SHORT;
    case TYPE_SHORT: return GL_SHORT;
    case TYPE_UINT: return GL_UNSIGNED_INT;
    case TYPE_INT: return GL_INT;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t idx_mode_to_gl(idx_mode_t mode)
{
  switch (mode) {
    case INDEX_TRIANGLES: return GL_TRIANGLES;
    case INDEX_TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
    case INDEX_TRIANGLE_FAN: return GL_TRIANGLE_FAN;
    case INDEX_LINES: return GL_LINES;
    case INDEX_LINE_STRIP: return GL_LINE_STRIP;
    case INDEX_LINE_LOOP: return GL_LINE_LOOP;
    case INDEX_POINTS: return GL_POINTS;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t img_fmt_to_gl(img_fmt_t format)
{
  switch (format) {
    case IMAGE_FORMAT_R8: return GL_RED;
    case IMAGE_FORMAT_RA8: return GL_RG;
    case IMAGE_FORMAT_RGB8: return GL_RGB;
    case IMAGE_FORMAT_RGBA8: return GL_RGBA;
    case IMAGE_FORMAT_INVALID: return GL_RGBA;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t tex_filter_to_gl(tex_filter_t filter)
{
  switch (filter) {
    case TEXTURE_FILTER_NEAREST: return GL_NEAREST;
    case TEXTURE_FILTER_NEAREST_MIPMAP: return GL_NEAREST_MIPMAP_LINEAR;
    case TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
      return GL_NEAREST_MIPMAP_NEAREST;
    case TEXTURE_FILTER_LINEAR: return GL_LINEAR;
    case TEXTURE_FILTER_LINEAR_MIPMAP: return GL_LINEAR_MIPMAP_LINEAR;
    case TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
  }
  return GL_INVALID_ENUM; // unreachable
}

uint32_t tex_wrap_to_gl(tex_wrap_t wrap)
{
  switch (wrap) {
    case TEXTURE_WRAP_REPEAT: return GL_REPEAT;
    case TEXTURE_WRAP_MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
    case TEXTURE_WRAP_CLAMP_EDGE: return GL_CLAMP_TO_EDGE;
    case TEXTURE_WRAP_CLAMP_BORDER: return GL_CLAMP_TO_BORDER;
  }
  return GL_INVALID_ENUM; // unreachable
}
