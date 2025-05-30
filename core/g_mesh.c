#include "g_mesh.h"

mesh_t mesh_create(shader_t shader, bool transparent)
{
  mesh_t m;
  m.transparent = transparent;
  m.vertices = NULL;
  m.vertex_count = 0;
  // m.indices = NULL;
  // m.index_count = 0;
  m.shader = shader;
  m.vao = NULL;
  m.vbo = NULL;
  // m.ebo = NULL;
  return m;
}

void mesh_destroy(const renderer_t* r, mesh_t* m)
{
  m->vertices = NULL;
  m->vertex_count = 0;
  // m->indices = NULL;
  // m->index_count = 0;
  m->shader = shader_none;

  if (m->vao != NULL) {
    vert_arr_destroy(r, m->vao);
  }
  if (m->vbo != NULL) {
    buf_obj_destroy(r, m->vbo);
    m->vbo = NULL;
  }
  // if (m->ebo != NULL) {
  //   buf_obj_destroy(r, m->ebo);
  //   m->ebo = NULL;
  // }
}

void mesh_finalize(const renderer_t* r, mesh_t* m, bool is_static)
{
  draw_mode_t mode = is_static ? draw_static : draw_dynamic;

  vert_fmt_t* fmt = &r->shader_fmts[m->shader];

  buf_obj_t* vbo = buf_obj_create(r, buf_arr);
  buf_obj_set_dat(r, vbo, m->vertices, fmt->stride * m->vertex_count, mode);
  buf_obj_bind(r, vbo);
  vert_arr_t* vao = vert_arr_create(r, m->shader);
  m->vao = vao;
  m->vbo = vbo;

  // if (m->index_count != 0) {
  //   buf_obj_t* ebo = buf_obj_create(r, buf_idx);
  //   buf_obj_set_dat(
  //     r,
  //     ebo,
  //     m->indices,
  //     sizeof(uint16_t) * m->index_count,
  //     mode
  //   );
  //   m->ebo = ebo;
  // }
}

void mesh_draw(
  renderer_t* r,
  const mesh_t* m,
  const tex_t* tex,
  const mat4_t transform)
{
  if (m->vao == NULL || m->vbo == NULL) {
    log_error("attempt to draw unfinalized mesh");
    return;
  }

  if (m->transparent) {
    // transparent things need to be deferred
    defer_draw_call(
      r, m->shader, transform, m->vertex_count, idx_triangles, tex, m->vao);
  } else {
    draw_call_t dc = create_draw_call(
      r, m->shader, transform, m->vertex_count, idx_triangles, tex, m->vao);
    setup_shader(r, &dc);
    vert_arr_draw(r, dc.vao, 0, dc.vertex_count, dc.idx_mode);
  }

}
