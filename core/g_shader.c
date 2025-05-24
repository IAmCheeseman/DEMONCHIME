#include "g_shader.h"

#include <string.h>

#include "c_mem.h"

static shader_var_t* find_var_in_arr(
  shader_var_t* vars, 
  size_t capacity,
  const char* name,
  size_t name_len,
  uint32_t name_hash)
{
  if (capacity == 0) return NULL;

  uint32_t index = name_hash & (capacity - 1);

  while (true) {
    shader_var_t* var = &vars[index];
    if (var->name == NULL) {
      return var;
    } else if (
      name_hash == var->hash &&
      name_len == var->len &&
      memcmp(name, var->name, name_len) == 0
    ) {
      return var;
    }

    index = (index + 1) & (capacity - 1);
  }
}

shader_var_t* shader_tab_find_var(
  shader_tab_t* t,
  const char* name,
  size_t len,
  uint32_t hash)
{
  return find_var_in_arr(t->vars, t->capacity, name, len, hash);
}

static void grow_shader_tab(shader_tab_t* t)
{
  size_t capacity = grow_capacity(t->capacity);

  shader_var_t* vars =
    (shader_var_t*)mem_alloc(sizeof(shader_var_t) * capacity);
  for (size_t i = 0; i < capacity; i++) {
    vars[i].name = NULL;
  }

  for (size_t i = 0; i < t->capacity; i++) {
    shader_var_t* var = &t->vars[i];
    if (var->name == NULL) continue; // empty
    
    shader_var_t* new_position = find_var_in_arr(
      vars,
      capacity,
      var->name,
      var->len,
      var->hash
    );
    *new_position = *var;
  }

  mem_destroy(t->vars);
  t->vars = vars;
  t->capacity = capacity;
}

shader_var_t* shader_tab_add_var(shader_tab_t* t, shader_var_t var)
{
  if (t->count + 1 > t->capacity * 0.75) {
    grow_shader_tab(t);
  }

  shader_var_t* ptr = shader_tab_find_var(t, var.name, var.len, var.hash);
  *ptr = var;
  t->count++;

  return ptr;
}

void shader_tab_destroy(shader_tab_t* t)
{
  for (size_t i = 0; i < t->capacity; i++) {
    shader_var_t* var = &t->vars[i];
    if (var->name == NULL) continue;
    mem_destroy(var->name);
  }
  mem_destroy(t->vars);

  t->vars = NULL;
  t->count = 0;
}

uint32_t hash_var_name(const char* str, size_t len)
{
  uint32_t hash = 2166136261u;
  for (size_t i = 0; i < len; i++) {
    hash ^= (uint8_t)str[i];
    hash *= 16777619;
  }
  return hash;
}

shader_t* shader_load_from_files(
  const renderer_t* r,
  vfs_t* vfs,
  const char* vert,
  const char* frag)
{
  return r->backend.shader_load_files(vfs, vert, frag);
}

shader_t* shader_load_from_src(
  const renderer_t* r,
  const char* vert,
  const char* frag)
{
  return r->backend.shader_load_src(vert, frag);
}

void shader_send_int(
  const renderer_t* r,
  shader_t* s,
  const char* name,
  int i)
{
  return r->backend.shader_send_int(s, name, i);
}

void shader_send_float(
  const renderer_t* r,
  shader_t* s,
  const char* name,
  float f)
{
  return r->backend.shader_send_float(s, name, f);
}

void shader_send_vec2f(
  const renderer_t* r,
  shader_t* s,
  const char* name,
  vec2f_t v)
{
  return r->backend.shader_send_vec2f(s, name, v);
}

void shader_send_vec2i(
  const renderer_t* r,
  shader_t* s,
  const char* name,
  vec2i_t v)
{
  return r->backend.shader_send_vec2i(s, name, v);
}

void shader_send_vec3f(
  const renderer_t* r,
  shader_t* s,
  const char* name,
  vec3f_t v)
{
  return r->backend.shader_send_vec3f(s, name, v);
}

void shader_send_vec3i(
  const renderer_t* r,
  shader_t* s,
  const char* name,
  vec3i_t v)
{
  return r->backend.shader_send_vec3i(s, name, v);
}

void shader_send_vec4f(
  const renderer_t* r,
  shader_t* s,
  const char* name,
  vec4f_t v)
{
  return r->backend.shader_send_vec4f(s, name, v);
}

void shader_send_vec4i(
  const renderer_t* r,
  shader_t* s,
  const char* name,
  vec4i_t v)
{
  return r->backend.shader_send_vec4i(s, name, v);
}

void shader_send_mat4(
  const renderer_t* r,
  shader_t* s,
  const char* name,
  mat4_t m)
{
  return r->backend.shader_send_mat4(s, name, m);
}

void shader_bind(const renderer_t* r, const shader_t* s)
{
  return r->backend.shader_bind(s);
}

void shader_destroy(const renderer_t* r, shader_t* s)
{
  return r->backend.shader_destroy(s);
}
