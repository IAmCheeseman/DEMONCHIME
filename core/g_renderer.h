#ifndef __engine_g_renderer__
#define __engine_g_renderer__

#include "c_engine.h"
#include "m_math.h"
#include "g_backend.h"

typedef struct buf_obj buf_obj_t;
typedef struct vert_fmt vert_fmt_t;
typedef struct vert_arr vert_arr_t;
typedef struct tex tex_t;

typedef struct draw_call
{
  shader_t shader;

  mat4_t transform;
  mat4_t view;
  mat4_t projection;
  float distance;

  size_t vertex_count;
  idx_mode_t idx_mode;

  const tex_t* tex;

  const vert_arr_t* vao;

  // TODO: Support EBOs
  // buf_obj_t* ebo;
} draw_call_t;

typedef struct draw_call_list
{
  draw_call_t* dc;
  size_t len;
  size_t capacity;
} draw_call_list_t;

typedef struct renderer
{
  mat4_t projection_3d;
  mat4_t projection_2d;
  mat4_t view;

  int draw_call_counter;
  int draw_call_count;

  vert_fmt_t* shader_fmts;
  void* shaders;
  shader_t active_shader;

  gfx_backend_t backend;

  draw_call_list_t deferred;
} renderer_t;

void init_renderer(engine_t* engine, gfx_backend_type_t backend);
void destroy_renderer(engine_t* engine);
draw_call_t create_draw_call(
  const renderer_t* renderer,
  shader_t shader,
  const mat4_t transform,
  size_t vertex_count,
  idx_mode_t idx_mode,
  const tex_t* tex,
  const vert_arr_t* vao);

void defer_draw_call(
  renderer_t* renderer,
  shader_t shader,
  const mat4_t transform,
  size_t vertex_count,
  idx_mode_t idx_mode,
  const tex_t* tex,
  const vert_arr_t* vao);

void flush_deferred(renderer_t* renderer);
void clear_bg(const renderer_t* renderer, float r, float g, float b);
void adjust_viewport(const renderer_t* r, vec2f_t size);
void set_depth_test(const renderer_t* r, bool test);

#endif
