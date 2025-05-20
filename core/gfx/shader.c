#include "shader.h"

#include <string.h>

#include "mem.h"

struct ShaderVar* ShaderTableFindVar(
  struct ShaderVar* vars, 
  size_t capacity,
  const char* name,
  size_t name_len,
  uint32_t name_hash)
{
  if (capacity == 0) return NULL;

  uint32_t index = name_hash & (capacity - 1);

  while (true) {
    struct ShaderVar* var = &vars[index];
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

static void GrowShaderTable(struct ShaderTable* t)
{
  size_t capacity = GrowCapacity(t->capacity);

  struct ShaderVar* vars =
    (struct ShaderVar*)Alloc(sizeof(struct ShaderVar) * capacity);
  for (size_t i = 0; i < capacity; i++) {
    vars[i].name = NULL;
  }

  for (size_t i = 0; i < t->capacity; i++) {
    struct ShaderVar* var = &t->vars[i];
    if (var->name == NULL) continue; // empty
    
    struct ShaderVar* new_position = ShaderTableFindVar(
      vars,
      capacity,
      var->name,
      var->len,
      var->hash
    );
    *new_position = *var;
  }

  Destroy(t->vars);
  t->vars = vars;
  t->capacity = capacity;
}

void ShaderTableAddVar(struct ShaderTable* t, struct ShaderVar var)
{
  if (t->count + 1 > t->capacity * 0.75) {
    GrowShaderTable(t);
  }

  struct ShaderVar* ptr = ShaderTableFindVar(
    t->vars,
    t->capacity,
    var.name,
    var.len,
    var.hash
  );
  *ptr = var;
  t->count++;
}

void ShaderTableDestroy(struct ShaderTable* t)
{
  for (size_t i = 0; i < t->capacity; i++) {
    struct ShaderVar* var = &t->vars[i];
    if (var->name == NULL) continue;
    Destroy(var->name);
  }
  Destroy(t->vars);

  t->vars = NULL;
  t->count = 0;
}

uint32_t HashVarName(const char* str, size_t len)
{
  uint32_t hash = 2166136261u;
  for (size_t i = 0; i < len; i++) {
    hash ^= (uint8_t)str[i];
    hash *= 16777619;
  }
  return hash;
}

struct Shader* ShaderLoadFromFiles(
  struct Renderer* r,
  struct Vfs* vfs,
  const char* vert,
  const char* frag)
{
  return r->backend.shader_load_files(vfs, vert, frag);
}

struct Shader* ShaderLoadFromSource(
  struct Renderer* r,
  const char* vert,
  const char* frag)
{
  return r->backend.shader_load_src(vert, frag);
}

void ShaderSendInt(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  int i)
{
  return r->backend.shader_send_int(s, name, i);
}

void ShaderSendFloat(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  float f)
{
  return r->backend.shader_send_float(s, name, f);
}

void ShaderSendVec2f(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec2f_t v)
{
  return r->backend.shader_send_vec2f(s, name, v);
}

void ShaderSendVec2i(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec2i_t v)
{
  return r->backend.shader_send_vec2i(s, name, v);
}

void ShaderSendVec3f(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec3f_t v)
{
  return r->backend.shader_send_vec3f(s, name, v);
}

void ShaderSendVec3i(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec3i_t v)
{
  return r->backend.shader_send_vec3i(s, name, v);
}

void ShaderSendVec4f(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec4f_t v)
{
  return r->backend.shader_send_vec4f(s, name, v);
}

void ShaderSendVec4i(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  vec4i_t v)
{
  return r->backend.shader_send_vec4i(s, name, v);
}

void ShaderSendMat4(
  struct Renderer* r,
  struct Shader* s,
  const char* name,
  mat4_t m)
{
  return r->backend.shader_send_mat4(s, name, m);
}

void ShaderBind(struct Renderer* r, struct Shader* s)
{
  return r->backend.shader_bind(s);
}

void ShaderDestroy(struct Renderer* r, struct Shader* s)
{
  return r->backend.shader_destroy(s);
}
