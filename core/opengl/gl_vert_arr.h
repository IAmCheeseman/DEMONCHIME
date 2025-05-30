#ifndef __engine_gfx_opengl_gl_vertex_array__
#define __engine_gfx_opengl_gl_vertex_array__

#include "include.h"

#include "g_vert_arr.h"

struct vert_arr_s
{
  uint32_t handle;
};

vert_arr_t* gl_vert_arr_create(const renderer_t* r, shader_t shader);
void gl_vert_arr_bind(const vert_arr_t* varr);
void gl_vert_arr_destroy(vert_arr_t* varr);
void gl_vert_arr_draw(
  const vert_arr_t* varr,
  size_t start,
  size_t count,
  idx_mode_t index_mode);
void gl_vert_arr_draw_idx(
  const vert_arr_t* varr,
  const buf_obj_t* ebo,
  size_t count,
  data_type_t type,
  idx_mode_t index_mode);

#endif
