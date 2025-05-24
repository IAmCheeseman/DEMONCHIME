#ifndef __engine_gfx_shader__
#define __engine_gfx_shader__

#include "include.h"
#include "gfx.h"
#include "gfx_texture.h"
#include "mathx.h"

typedef struct shader_var_s
{
  char* name;
  size_t len;
  uint32_t hash;
  int loc;
} shader_var_t;

typedef struct shader_table_s
{
  shader_var_t* vars;
  size_t count;
  size_t capacity;
} shader_tab_t;

typedef struct shader_s shader_t;

shader_var_t* shader_tab_add_var(shader_tab_t* t, shader_var_t var);
shader_var_t* shader_tab_find_var(
  shader_tab_t* t,
  const char* name,
  size_t len,
  uint32_t hash);
void shader_tab_destroy(shader_tab_t* t);
uint32_t hash_var_name(const char* name, size_t len);

shader_t* shader_load_from_files(
  const renderer_t* r,
  vfs_t* vfs,
  const char* vert,
  const char* frag);
shader_t* shader_load_from_src(
  const renderer_t* r,
  const char* vert,
  const char* frag);
void shader_send_int(
  const renderer_t* r, shader_t* s, const char* name, int i);
void shader_send_float(
  const renderer_t* r, shader_t* s, const char* name, float f);
void shader_send_vec2f(
  const renderer_t* r, shader_t* s, const char* name, vec2f_t v);
void shader_send_vec2i(
  const renderer_t* r, shader_t* s, const char* name, vec2i_t v);
void shader_send_vec3f(
  const renderer_t* r, shader_t* s, const char* name, vec3f_t v);
void shader_send_vec3i(
  const renderer_t* r, shader_t* s, const char* name, vec3i_t v);
void shader_send_vec4f(
  const renderer_t* r, shader_t* s, const char* name, vec4f_t v);
void shader_send_vec4i(
  const renderer_t* r, shader_t* s, const char* name, vec4i_t v);
void shader_send_mat4(
  const renderer_t* r, shader_t* s, const char* name, mat4_t m);
void shader_bind(const renderer_t* r, const shader_t* s);
void shader_destroy(const renderer_t* r, shader_t* s);

#endif
