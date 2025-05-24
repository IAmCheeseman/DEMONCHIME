#include "gl_shader.h"

#include <glad/glad.h>

#include "c_mem.h"

#define UNIFORM_NAME_MAX 512

static void find_shader_uniforms(shader_t* s)
{
  s->uniforms.vars = NULL;
  s->uniforms.count = 0;
  s->uniforms.capacity = 0;

  int uniform_count;
  glGetProgramiv(s->handle, GL_ACTIVE_UNIFORMS, &uniform_count);

  for (int i = 0; i < uniform_count; i++) {
    int name_len;
    char name[UNIFORM_NAME_MAX];
    shader_var_t var;
    var.loc = i;
    glGetActiveUniform(
       s->handle, i, UNIFORM_NAME_MAX, &name_len, NULL, NULL, name);

    var.name = (char*)mem_alloc(sizeof(char) * (name_len + 1));
    memcpy(var.name, name, name_len);
    var.name[name_len] = '\0';
    var.len = name_len;
    var.hash = hash_var_name(name, name_len);

    shader_tab_add_var(&s->uniforms, var);
  }
}

static void find_shader_attrs(shader_t* s)
{
  s->attrs.vars = NULL;
  s->attrs.count = 0;
  s->attrs.capacity = 0;

  int attr_count;
  glGetProgramiv(s->handle, GL_ACTIVE_ATTRIBUTES, &attr_count);

  for (int i = 0; i < attr_count; i++) {
    int name_len;
    char name[UNIFORM_NAME_MAX];
    shader_var_t var;
    var.loc = i;
    glGetActiveAttrib(
       s->handle, i, UNIFORM_NAME_MAX, &name_len, NULL, NULL, name);

    var.name = (char*)mem_alloc(sizeof(char) * (name_len + 1));
    memcpy(var.name, name, name_len);
    var.name[name_len] = '\0';
    var.len = name_len;
    var.hash = hash_var_name(name, name_len);

    shader_tab_add_var(&s->attrs, var);
  }
}

static shader_t* shader_create(uint32_t vert, uint32_t frag)
{
  shader_t* s = (shader_t*)mem_alloc(sizeof(shader_t));

  uint32_t handle = glCreateProgram();

  s->handle = handle;

  glAttachShader(handle, vert);
  glAttachShader(handle, frag);
  glLinkProgram(handle);

  glDeleteShader(vert);
  glDeleteShader(frag);

  int status;
  glGetProgramiv(handle, GL_LINK_STATUS, &status);
  if (!status) {
    char msg[512];
    glGetProgramInfoLog(handle, 512, NULL, msg);
    log_fatal(
      1, "[" text_dark_gray "shader" text_normal "] link error: %s", msg);
  }

  s->uniforms.vars = NULL;
  s->uniforms.count = 0;
  s->uniforms.capacity = 0;
  s->attrs.vars = NULL;
  s->attrs.count = 0;
  s->attrs.capacity = 0;
  //FindShaderUniforms(s);
  //FindShaderAttribs(s);

  log_debug("created shader %d", handle);

  return s;
}

static uint32_t compile_src(const char* name, const char* src, uint32_t type)
{
  uint32_t h = glCreateShader(type);
  glShaderSource(h, 1, &src, NULL);
  glCompileShader(h);

  int status;
  glGetShaderiv(h, GL_COMPILE_STATUS, &status);
  if (!status) {
    char msg[512];
    glGetShaderInfoLog(h, 512, NULL, msg);
    log_fatal(1, "[" text_dark_gray "shader" text_normal "] %s: %s", name, msg);
  }

  return h;
}

shader_t* gl_shader_load_from_files(
  vfs_t* vfs,
  const char* vert,
  const char* frag)
{
  char* vsrc = vfs_read_txt(vfs, vert, NULL);//ReadFile(vert);
  if (!vsrc) log_fatal(1, "could not load vertex shader '%s'", vert);
  char* fsrc = vfs_read_txt(vfs, frag, NULL);//ReadFile(frag);
  if (!fsrc) log_fatal(1, "could not load fragment shader '%s'", frag);
  uint32_t vertex_handle = compile_src(vert, vsrc, GL_VERTEX_SHADER);
  uint32_t fragment_handle = compile_src(frag, fsrc, GL_FRAGMENT_SHADER);
  mem_destroy(vsrc);
  mem_destroy(fsrc);

  return shader_create(vertex_handle, fragment_handle);
}

shader_t* gl_shader_load_from_src(const char* vert, const char* frag)
{
  uint32_t vertex_handle = compile_src(
    "embedded vertex",
    vert,
    GL_VERTEX_SHADER
  );
  uint32_t fragment_handle = compile_src(
    "embedded fragment",
    frag,
    GL_FRAGMENT_SHADER
  );

  return shader_create(vertex_handle, fragment_handle);
}

static shader_var_t* get_uniform(shader_t* s, const char* name)
{
  size_t len = strlen(name);
  uint32_t hash = hash_var_name(name, len);
  shader_var_t* var = shader_tab_find_var(&s->uniforms, name, len, hash);

  if (var == NULL || var->name == NULL) {
    int loc = glGetUniformLocation(s->handle, name);
    if (loc == -1) log_fatal(1, "uniform '%s' does not exist", name);

    shader_var_t new_var;
    new_var.loc = loc;
    new_var.name = (char*)mem_alloc(sizeof(char) * (len + 1));
    strcpy(new_var.name, name);
    new_var.len = len;
    new_var.hash = hash;
    return shader_tab_add_var(&s->uniforms, new_var);
  }
  return var;
}

void gl_shader_send_int(shader_t* s, const char* name, int i)
{
  shader_var_t* ptr = get_uniform(s, name);
  glUniform1i(ptr->loc, i);
}

void gl_shader_send_float(shader_t* s, const char* name, float f)
{
  shader_var_t* ptr = get_uniform(s, name);
  glUniform1f(ptr->loc, f);
}

void gl_shader_send_vec2f(shader_t* s, const char* name, vec2f_t v)
{
  shader_var_t* ptr = get_uniform(s, name);
  glUniform2f(ptr->loc, v.x, v.y);
}

void gl_shader_send_vec2i(shader_t* s, const char* name, vec2i_t v)
{
  shader_var_t* ptr = get_uniform(s, name);
  glUniform2i(ptr->loc, v.x, v.y);
}

void gl_shader_send_vec3f(shader_t* s, const char* name, vec3f_t v)
{
  shader_var_t* ptr = get_uniform(s, name);
  glUniform3f(ptr->loc, v.x, v.y, v.z);
}

void gl_shader_send_vec3i(shader_t* s, const char* name, vec3i_t v)
{
  shader_var_t* ptr = get_uniform(s, name);
  glUniform3i(ptr->loc, v.x, v.y, v.z);
}

void gl_shader_send_vec4f(shader_t* s, const char* name, vec4f_t v)
{
  shader_var_t* ptr = get_uniform(s, name);
  glUniform4f(ptr->loc, v.x, v.y, v.z, v.w);
}

void gl_shader_send_vec4i(shader_t* s, const char* name, vec4i_t v)
{
  shader_var_t* ptr = get_uniform(s, name);
  glUniform4i(ptr->loc, v.x, v.y, v.z, v.w);
}

void gl_shader_send_mat4(shader_t* s, const char* name, mat4_t m)
{
  shader_var_t* ptr = get_uniform(s, name);
  glUniformMatrix4fv(ptr->loc, 1, GL_FALSE, m);
}

void gl_shader_bind(const shader_t* s)
{
  uint32_t handle = 0;
  if (s != NULL) handle = s->handle;
  glUseProgram(handle);
}

void gl_shader_destroy(shader_t* s)
{
  glDeleteProgram(s->handle);
  log_debug("destroyed shader %d", s->handle);

  shader_tab_destroy(&s->uniforms);
  shader_tab_destroy(&s->attrs);

  mem_destroy(s);
}
