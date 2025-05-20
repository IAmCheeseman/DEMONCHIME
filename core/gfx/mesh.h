#ifndef __engine_mesh__
#define __engine_mesh__

#include "include.h"
#include "buffer_object.h"
#include "vertex_array.h"

typedef struct mesh_s
{
  void* vertices;
  size_t vertex_count;
  uint16_t* indices;
  size_t index_count;

  vert_fmt_t* fmt;

  vert_arr_t* vao;
  buf_obj_t* vbo;
  buf_obj_t* ebo;
} mesh_t;

mesh_t mesh_create(vert_fmt_t* fmt);
void mesh_destroy(renderer_t* r, mesh_t* m);
void mesh_finalize(renderer_t* r, mesh_t* m, bool is_static);
void mesh_draw(renderer_t* r, mesh_t* m);

#endif
