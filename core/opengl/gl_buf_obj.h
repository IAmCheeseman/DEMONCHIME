#ifndef __engine_gfx_opengl_gl_buffer_object__
#define __engine_gfx_opengl_gl_buffer_object__

#include "include.h"
#include "g_buf_obj.h"

struct buf_obj_s
{
  buf_obj_type_t type;
  uint32_t handle;
};

buf_obj_t* gl_buf_obj_create(buf_obj_type_t type);
void gl_buf_obj_destroy(buf_obj_t* buf);
void gl_buf_obj_bind(const buf_obj_t* buf);
void gl_buf_obj_unbind(buf_obj_type_t type);
void gl_buf_obj_set_dat(
  buf_obj_t* buf,
  void* data,
  size_t size,
  draw_mode_t mode
);

#endif
