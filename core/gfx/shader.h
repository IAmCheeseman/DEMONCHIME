#ifndef __engine_gfx_shader__
#define __engine_gfx_shader__

#include "include.h"
#include "gfx.h"
#include "math/vec2f.h"
#include "math/vec3f.h"
#include "math/vec4f.h"
#include "math/vec2i.h"
#include "math/vec3i.h"
#include "math/vec4i.h"
#include "math/mat4.h"
#include "texture.h"

typedef struct shader_var_s
{
  char* name;
  size_t len;
  uint32_t hash;

  int loc;
  int count;
  uint32_t type;
} shader_var_t;

typedef struct shader_table_s
{
  shader_var_t* vars;
  size_t count;
  size_t capacity;
} shader_tab_t;

typedef struct shader_s shader_t;

void shader_tab_add_var(shader_tab_t* t, shader_var_t var);
shader_var_t* shader_tab_find_var(
  shader_var_t* vars, 
  size_t capacity,
  const char* name,
  size_t name_len,
  uint32_t name_hash
);
void shader_tab_destroy(shader_tab_t* t);
uint32_t hash_var_name(const char* name, size_t len);

shader_t* shader_load_from_files(
  renderer_t* r,
  vfs_t* vfs,
  const char* vert,
  const char* frag
);
shader_t* shader_load_from_src(
  renderer_t* r,
  const char* vert,
  const char* frag
);
void shader_send_int(renderer_t* r, shader_t* s, const char* name, int i);
void shader_send_float(renderer_t* r, shader_t* s, const char* name, float f);
void shader_send_vec2f(renderer_t* r, shader_t* s, const char* name, vec2f_t v);
void shader_send_vec2i(renderer_t* r, shader_t* s, const char* name, vec2i_t v);
void shader_send_vec3f(renderer_t* r, shader_t* s, const char* name, vec3f_t v);
void shader_send_vec3i(renderer_t* r, shader_t* s, const char* name, vec3i_t v);
void shader_send_vec4f(renderer_t* r, shader_t* s, const char* name, vec4f_t v);
void shader_send_vec4i(renderer_t* r, shader_t* s, const char* name, vec4i_t v);
void shader_send_mat4(renderer_t* r, shader_t* s, const char* name, mat4_t m);
void shader_bind(renderer_t* r, shader_t* s);
void shader_destroy(renderer_t* r, shader_t* s);

#endif
