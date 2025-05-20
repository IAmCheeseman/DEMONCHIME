#include "mesh.h"

mesh_t mesh_create(vert_fmt_t* fmt)
{
  mesh_t m;
  m.vertices = NULL;
  m.vertex_count = 0;
  m.indices = NULL;
  m.index_count = 0;
  m.fmt = fmt;
  m.vao = NULL;
  m.vbo = NULL;
  m.ebo = NULL;
  return m;
}

void mesh_destroy(renderer_t* r, mesh_t* m)
{
  m->vertices = NULL;
  m->vertex_count = 0;
  m->indices = NULL;
  m->index_count = 0;
  m->fmt = NULL;

  if (m->vao != NULL) {
    vert_arr_destroy(r, m->vao);
  }
  if (m->vbo != NULL) {
    buf_obj_destroy(r, m->vbo);
    m->vbo = NULL;
  }
  if (m->ebo != NULL) {
    buf_obj_destroy(r, m->ebo);
    m->ebo = NULL;
  }
}

void mesh_finalize(renderer_t* r, mesh_t* m, bool is_static)
{
  draw_mode_t mode = is_static ? DRAW_STATIC : DRAW_DYNAMIC;

  buf_obj_t* vbo = buf_obj_create(r, BUFFER_ARRAY);
  buf_obj_set_dat(r, vbo, m->vertices, m->fmt->stride * m->vertex_count, mode);
  buf_obj_bind(r, vbo);
  vert_arr_t* vao = vert_arr_create(r, m->fmt);
  m->vao = vao;
  m->vbo = vbo;

  if (m->index_count != 0) {
    buf_obj_t* ebo = buf_obj_create(r, BUFFER_INDEX);
    buf_obj_set_dat(
      r,
      ebo,
      m->indices,
      sizeof(uint16_t) * m->index_count,
      mode
    );
    m->ebo = ebo;
  }
}

void mesh_draw(renderer_t* r, mesh_t* m)
{
  if (m->vao == NULL || m->vbo == NULL) {
    log_error("attempt to draw unfinalized mesh");
    return;
  }

  if (m->ebo != NULL) {
    vert_arr_draw_idx(
      r,
      m->vao,
      m->ebo,
      m->index_count,
      TYPE_USHORT,
      INDEX_TRIANGLES
    );
  } else {
    vert_arr_draw(r, m->vao, 0, m->vertex_count, INDEX_TRIANGLES);
  }
}
