#include "g_renderer.h"

#ifdef bse_allow_opengl
#include "opengl/gl_gfx.h"
#endif

#include "g_shader.h"
#include "g_vert_arr.h"
#include "c_mem.h"

static void init_draw_call_list(draw_call_list_t* dclist)
{
  dclist->dc = NULL;
  dclist->len = 0;
  dclist->capacity = 0;
}

draw_call_t create_draw_call(
  const renderer_t* renderer,
  shader_t shader,
  const mat4_t transform,
  size_t vertex_count,
  idx_mode_t idx_mode,
  const tex_t* tex,
  const vert_arr_t* vao)
{
  draw_call_t dc;
  dc.shader = shader;
  memcpy(dc.transform, transform, sizeof(mat4_t));
  memcpy(dc.view, renderer->view, sizeof(mat4_t));
  if (shader == shader_2d || shader == shader_text)
    memcpy(dc.projection, renderer->projection_2d, sizeof(mat4_t));
  else
    memcpy(dc.projection, renderer->projection_3d, sizeof(mat4_t));
  dc.vertex_count = vertex_count;
  dc.idx_mode = idx_mode;
  dc.tex = tex;
  dc.vao = vao;
  dc.distance = 0;
  return dc;
}

void defer_draw_call(
  renderer_t* renderer,
  shader_t shader,
  const mat4_t transform,
  size_t vertex_count,
  idx_mode_t idx_mode,
  const tex_t* tex,
  const vert_arr_t* vao)
{
  draw_call_t dc = create_draw_call(
    renderer, shader, transform, vertex_count, idx_mode, tex, vao);

  mat4_t vm;
  mat4_mult(vm, dc.view, transform);
  dc.distance = vm[14]; // 14 is the depth

  if (renderer->deferred.len + 1 > renderer->deferred.capacity) {
    renderer->deferred.capacity = grow_capacity(renderer->deferred.capacity);
    renderer->deferred.dc = (draw_call_t*)mem_realloc(
      renderer->deferred.dc,
      sizeof(draw_call_t) * renderer->deferred.capacity);
  }

  renderer->deferred.dc[renderer->deferred.len++] = dc;
}

static void flush_list(const renderer_t* renderer, draw_call_list_t* dclist)
{
  for (size_t i = 0; i < dclist->len; i++) {
    draw_call_t* dc = &dclist->dc[i];
    setup_shader(renderer, dc);
    vert_arr_draw(renderer, dc->vao, 0, dc->vertex_count, dc->idx_mode);
  }
  dclist->len = 0;
}

void sort_draw_calls(draw_call_list_t* list)
{
  // bubble sort! :D
  // it should be fine enough for this, and i don't care enough to figure out
  // how other algorithms work
  if (list->len == 0) return;

  while (true) {
    bool swapped = false;
    for (size_t i = 0; i < list->len - 1; i++) {
      if (list->dc[i].distance > list->dc[i+1].distance) {
        draw_call_t tmp = list->dc[i];
        list->dc[i] = list->dc[i+1];
        list->dc[i+1] = tmp;
        swapped = true;
      }
    }
    if (!swapped) break;
  }
}

void flush_deferred(renderer_t* renderer)
{
  sort_draw_calls(&renderer->deferred);
  flush_list(renderer, &renderer->deferred);
}

vert_attr_t default_attrs[] = {
  {type_float, 3}, // pos
  {type_float, 3}, // normal
  {type_float, 2}, // uv
  {type_float, 4}, // color
};

vert_attr_t billboard_attrs[] = {
  {type_float, 3}, // pos
  {type_float, 2}, // uv
  {type_float, 4}, // color
};

vert_attr_t _2d_attrs[] = {
  {type_float, 2}, // pos
  {type_float, 2}, // uv
  {type_float, 4}, // color
};

void init_renderer(engine_t* engine, gfx_backend_type_t backend)
{
  renderer_t* renderer = (renderer_t*)mem_alloc(sizeof(renderer_t));
  renderer->shader_fmts = NULL;
  renderer->shaders = NULL;

  engine->renderer = renderer;

  switch (backend) {
    case gfx_backend_opengl:
    #ifdef bse_allow_opengl
      gl_init_backend(engine);
    #else
      log_fatal(1, "opengl is not supported on this system");
    #endif
      break;
    default:
      log_fatal(1, "invalid graphics backend");
      break;
  }

  mat4_identity(renderer->projection_3d);
  mat4_identity(renderer->projection_2d);
  mat4_identity(renderer->view);

  init_draw_call_list(&renderer->deferred);

  init_shaders(renderer, engine->vfs);

  vert_fmt_t* fmts = (vert_fmt_t*)mem_alloc(sizeof(vert_fmt_t) * shader_count);
  fmts[shader_default] = vert_fmt_create(default_attrs, 4);
  fmts[shader_billboard] = vert_fmt_create(billboard_attrs, 3);
  fmts[shader_2d] = vert_fmt_create(_2d_attrs, 3);
  fmts[shader_text] = vert_fmt_create(_2d_attrs, 3);

  renderer->shader_fmts = fmts;
}

void destroy_renderer(engine_t* engine)
{
  if (engine->renderer->shaders) mem_destroy(engine->renderer->shaders);
  if (engine->renderer->shader_fmts)
    mem_destroy(engine->renderer->shader_fmts);

  if (engine->renderer->deferred.dc) {
    mem_destroy(engine->renderer->deferred.dc);
    engine->renderer->deferred.capacity = 0;
    engine->renderer->deferred.len = 0;
  }
  mem_destroy(engine->renderer);
}

void clear_bg(const renderer_t* renderer, float r, float g, float b)
{
  return renderer->backend.clear_background(r, g, b);
}

void adjust_viewport(const renderer_t* r, vec2f_t size)
{
  return r->backend.adjust_viewport(size);
}

void set_depth_test(const renderer_t* r, bool test)
{
  return r->backend.set_depth_test(test);
}

