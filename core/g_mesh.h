#ifndef __engine_mesh__
#define __engine_mesh__

#include "include.h"
#include "g_buf_obj.h"
#include "g_vert_arr.h"

typedef struct mesh_s
{
  void* vertices;
  size_t vertex_count;
  uint16_t* indices;
  size_t index_count;

  const vert_fmt_t* fmt;

  vert_arr_t* vao;
  buf_obj_t* vbo;
  buf_obj_t* ebo;
} mesh_t;

mesh_t mesh_create(const vert_fmt_t* fmt);
void mesh_destroy(const renderer_t* r, mesh_t* m);
void mesh_finalize(const renderer_t* r, mesh_t* m, bool is_static);
void mesh_draw(const renderer_t* r, const mesh_t* m);

#endif
