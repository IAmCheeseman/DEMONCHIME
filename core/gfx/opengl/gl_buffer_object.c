#include "gl_buffer_object.h"

#include <glad/glad.h>

#include "mem.h"
#include "gl_type_conv.h"

buf_obj_t* gl_buf_obj_create(buf_obj_type_t type)
{
  buf_obj_t* buf =
    (buf_obj_t*)mem_alloc(sizeof(buf_obj_t));
  buf->type = type;
  glGenBuffers(1, &buf->handle);
  log_debug("created buffer object %d", buf->handle);
  return buf;
}

void gl_buf_obj_destroy(buf_obj_t* buf)
{
  glDeleteBuffers(1, &buf->handle);
  log_debug("destroyed buffer object %d", buf->handle);
  mem_destroy(buf);
}

void gl_buf_obj_bind(const buf_obj_t* buf)
{
  glBindBuffer(buf_obj_type_to_gl(buf->type), buf->handle);
}

void gl_buf_obj_unbind(buf_obj_type_t type)
{
  glBindBuffer(buf_obj_type_to_gl(type), 0);
}

void gl_buf_obj_set_dat(
  buf_obj_t* buf,
  void* data,
  size_t size,
  draw_mode_t mode)
{
  uint32_t gl_mode = draw_mode_to_gl(mode);
  uint32_t type = buf_obj_type_to_gl(buf->type);
  glBindBuffer(type, buf->handle);
  glBufferData(type, size, data, gl_mode);
  glBindBuffer(type, 0);
}
