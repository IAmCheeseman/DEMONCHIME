#include "gl_shader.h"

#include <glad/glad.h>

#include "mem.h"

#define UNIFORM_NAME_MAX 512

static void FindShaderUniforms(struct Shader* s)
{
  s->uniforms.vars = NULL;
  s->uniforms.count = 0;
  s->uniforms.capacity = 0;

  int uniform_count;
  glGetProgramiv(s->handle, GL_ACTIVE_UNIFORMS, &uniform_count);

  for (int i = 0; i < uniform_count; i++) {
    int name_len;
    char name[UNIFORM_NAME_MAX];
    struct ShaderVar var;
    var.loc = i;
    glGetActiveUniform(
       s->handle, i, UNIFORM_NAME_MAX, &name_len, &var.count, &var.type, name);

    var.name = (char*)Alloc(sizeof(char) * (name_len + 1));
    memcpy(var.name, name, name_len);
    var.name[name_len] = '\0';
    var.len = name_len;
    var.hash = HashVarName(name, name_len);

    ShaderTableAddVar(&s->uniforms, var);
  }
}

static void FindShaderAttribs(struct Shader* s)
{
  s->attrs.vars = NULL;
  s->attrs.count = 0;
  s->attrs.capacity = 0;

  int attr_count;
  glGetProgramiv(s->handle, GL_ACTIVE_ATTRIBUTES, &attr_count);

  for (int i = 0; i < attr_count; i++) {
    int name_len;
    char name[UNIFORM_NAME_MAX];
    struct ShaderVar var;
    var.loc = i;
    glGetActiveAttrib(
       s->handle, i, UNIFORM_NAME_MAX, &name_len, &var.count, &var.type, name);

    var.name = (char*)Alloc(sizeof(char) * (name_len + 1));
    memcpy(var.name, name, name_len);
    var.name[name_len] = '\0';
    var.len = name_len;
    var.hash = HashVarName(name, name_len);

    ShaderTableAddVar(&s->attrs, var);
  }
}

static struct Shader* ShaderCreate(uint32_t vert, uint32_t frag)
{
  struct Shader* s = (struct Shader*)Alloc(sizeof(struct Shader));

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
    LogFatal(1, "[" TEXT_DARK_GRAY "shader" TEXT_NORMAL "] link error: %s", msg);
  }

  s->uniforms.vars = NULL;
  s->uniforms.count = 0;
  s->uniforms.capacity = 0;
  s->attrs.vars = NULL;
  s->attrs.count = 0;
  s->attrs.capacity = 0;
  //FindShaderUniforms(s);
  //FindShaderAttribs(s);

  LogDebug("created shader %d", handle);

  return s;
}

static uint32_t CompileSource(const char* name, const char* src, uint32_t type)
{
  uint32_t h = glCreateShader(type);
  glShaderSource(h, 1, &src, NULL);
  glCompileShader(h);

  int status;
  glGetShaderiv(h, GL_COMPILE_STATUS, &status);
  if (!status) {
    char msg[512];
    glGetShaderInfoLog(h, 512, NULL, msg);
    LogFatal(1, "[" TEXT_DARK_GRAY "shader" TEXT_NORMAL"] %s: %s", name, msg);
  }

  return h;
}

struct Shader* gl_ShaderLoadFromFiles(
  struct Vfs* vfs,
  const char* vert,
  const char* frag)
{
  char* vsrc = VfsReadTxtFile(vfs, vert, NULL);//ReadFile(vert);
  if (!vsrc) LogFatal(1, "could not load vertex shader '%s'", vert);
  char* fsrc = VfsReadTxtFile(vfs, frag, NULL);//ReadFile(frag);
  if (!fsrc) LogFatal(1, "could not load fragment shader '%s'", frag);
  uint32_t vertex_handle = CompileSource(vert, vsrc, GL_VERTEX_SHADER);
  uint32_t fragment_handle = CompileSource(frag, fsrc, GL_FRAGMENT_SHADER);
  Destroy(vsrc);
  Destroy(fsrc);

  return ShaderCreate(vertex_handle, fragment_handle);
}

struct Shader* gl_ShaderLoadFromSource(const char* vert, const char* frag)
{
  uint32_t vertex_handle = CompileSource(
    "embedded vertex",
    vert,
    GL_VERTEX_SHADER
  );
  uint32_t fragment_handle = CompileSource(
    "embedded fragment",
    frag,
    GL_FRAGMENT_SHADER
  );

  return ShaderCreate(vertex_handle, fragment_handle);
}

static struct ShaderVar* GetUniform(struct Shader* s, const char* name)
{
  size_t len = strlen(name);
  uint32_t hash = HashVarName(name, len);
  struct ShaderVar* var = ShaderTableFindVar(
    s->uniforms.vars,
    s->uniforms.capacity,
    name,
    len,
    hash
  );
  if (var == NULL || var->name == NULL) {
    int loc = glGetUniformLocation(s->handle, name);
    if (loc == -1) LogFatal(1, "uniform '%s' does not exist", name);

    struct ShaderVar new_var;
    new_var.loc = loc;

    new_var.name = (char*)Alloc(sizeof(char) * (len + 1));
    memcpy(new_var.name, name, len);
    new_var.name[len] = '\0';
    new_var.len = len;
    new_var.hash = hash;
    new_var.type = TYPE_FLOAT;
    new_var.count = 1;

    ShaderTableAddVar(&s->uniforms, new_var);
    return GetUniform(s, name);
  }
  return var;
}

void gl_ShaderSendInt(struct Shader* s, const char* name, int i)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform1i(ptr->loc, i);
}

void gl_ShaderSendFloat(struct Shader* s, const char* name, float f)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform1f(ptr->loc, f);
}

void gl_ShaderSendVec2f(struct Shader* s, const char* name, vec2f_t v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform2f(ptr->loc, v.x, v.y);
}

void gl_ShaderSendVec2i(struct Shader* s, const char* name, vec2i_t v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform2i(ptr->loc, v.x, v.y);
}

void gl_ShaderSendVec3f(struct Shader* s, const char* name, vec3f_t v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform3i(ptr->loc, v.x, v.y, v.z);
}

void gl_ShaderSendVec3i(struct Shader* s, const char* name, vec3i_t v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform3i(ptr->loc, v.x, v.y, v.z);
}

void gl_ShaderSendVec4f(struct Shader* s, const char* name, vec4f_t v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform4i(ptr->loc, v.x, v.y, v.z, v.w);
}

void gl_ShaderSendVec4i(struct Shader* s, const char* name, vec4i_t v)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniform4i(ptr->loc, v.x, v.y, v.z, v.w);
}

void gl_ShaderSendMat4(struct Shader* s, const char* name, mat4_t m)
{
  struct ShaderVar* ptr = GetUniform(s, name);
  glUniformMatrix4fv(ptr->loc, 1, GL_FALSE, m);
}

void gl_ShaderBind(struct Shader* s)
{
  uint32_t handle = 0;
  if (s != NULL) handle = s->handle;
  glUseProgram(handle);
}

void gl_ShaderDestroy(struct Shader* s)
{
  glDeleteProgram(s->handle);
  LogDebug("destroyed shader %d", s->handle);

  ShaderTableDestroy(&s->uniforms);
  ShaderTableDestroy(&s->attrs);

  Destroy(s);
}
