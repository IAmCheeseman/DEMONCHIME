#include "gl_shader.h"

#include <glad/glad.h>

#include "c_mem.h"
#include "gl_vert_arr.h"
#include "gl_tex.h"

#define max_locs 32

#define transform_loc 0
#define view_loc 1
#define projection_loc 2
#define tex_loc 3

#define tex_size_loc 4
#define scale_loc 5

typedef struct program_s
{
  uint32_t handle;
  int locs[max_locs];
} program_t;

uint32_t load_shader(vfs_t* vfs, const char* path, uint32_t type)
{
  uint32_t h = glCreateShader(type);
  char* src = vfs_read_txt(vfs, path, NULL);
  if (!src) log_fatal(1, "could not load shader '%s'", path);
  const char* s = src;
  glShaderSource(h, 1, &s, NULL);
  glCompileShader(h);
  mem_destroy(src);

  int status;
  glGetShaderiv(h, GL_COMPILE_STATUS, &status);
  if (!status) {
    char msg[512];
    glGetShaderInfoLog(h, 512, NULL, msg);
    log_fatal(
      1,
      "[" text_dark_gray "shader" text_normal "] '%s': %s", path, msg);
  }

  return h;
}

uint32_t load_program(vfs_t* vfs, const char* vert_path, const char* frag_path)
{
  uint32_t vert = load_shader(vfs, vert_path, GL_VERTEX_SHADER);
  uint32_t frag = load_shader(vfs, frag_path, GL_FRAGMENT_SHADER);

  uint32_t program = glCreateProgram();
  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glLinkProgram(program);

  glDeleteShader(vert);
  glDeleteShader(frag);

  int status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    char msg[512];
    glGetProgramInfoLog(program, 512, NULL, msg);
    log_fatal(
      1,
      "[" text_dark_gray "shader" text_normal "] link error '%s'+'%s': %s",
      vert_path, frag_path, msg);
  }

  log_debug("created shader %d", program);

  return program;
}

void gl_init_shaders(renderer_t* r, vfs_t* vfs)
{
  program_t* progs = (program_t*)mem_alloc(sizeof(program_t) * shader_count);

  program_t def;
  def.handle = load_program(vfs, "res/vdefault.glsl", "res/fdefault.glsl");
  memset(def.locs, 0, sizeof(def.locs));
  def.locs[transform_loc] = glGetUniformLocation(def.handle, "m");
  def.locs[view_loc] = glGetUniformLocation(def.handle, "v");
  def.locs[projection_loc] = glGetUniformLocation(def.handle, "p");
  def.locs[tex_loc] = glGetUniformLocation(def.handle, "tex");
  progs[shader_default] = def;

  program_t bb;
  bb.handle = load_program(vfs, "res/vbillboard.glsl", "res/fdefault.glsl");
  memset(bb.locs, 0, sizeof(bb.locs));
  bb.locs[transform_loc] = glGetUniformLocation(bb.handle, "m");
  bb.locs[view_loc] = glGetUniformLocation(bb.handle, "v");
  bb.locs[projection_loc] = glGetUniformLocation(bb.handle, "p");
  bb.locs[tex_loc] = glGetUniformLocation(bb.handle, "tex");
  bb.locs[tex_size_loc] = glGetUniformLocation(bb.handle, "tex_size");
  bb.locs[scale_loc] = glGetUniformLocation(bb.handle, "scale");
  progs[shader_billboard] = bb;

  program_t _2d;
  _2d.handle = load_program(vfs, "res/v2d.glsl", "res/f2d.glsl");
  memset(_2d.locs, 0, sizeof(_2d.locs));
  _2d.locs[projection_loc] = glGetUniformLocation(_2d.handle, "p");
  _2d.locs[tex_loc] = glGetUniformLocation(_2d.handle, "tex");
  progs[shader_2d] = _2d;

  program_t text;
  text.handle = load_program(vfs, "res/v2d.glsl", "res/ftext.glsl");
  memset(text.locs, 0, sizeof(text.locs));
  text.locs[projection_loc] = glGetUniformLocation(text.handle, "p");
  text.locs[tex_loc] = glGetUniformLocation(text.handle, "tex");
  progs[shader_text] = text;

  r->shaders = progs;
}

void gl_destroy_shaders(renderer_t* r)
{
  program_t* progs = (program_t*)r->shaders;
  glDeleteProgram(progs[shader_default].handle);
  glDeleteProgram(progs[shader_billboard].handle);
  glDeleteProgram(progs[shader_2d].handle);
  glDeleteProgram(progs[shader_text].handle);
  mem_destroy(progs);
  r->shaders = NULL;
}

void gl_set_active_shader(const renderer_t* r, uint8_t shader)
{
  program_t* progs = (program_t*)r->shaders;
  glUseProgram(progs[shader].handle);
}

void gl_setup_2d(
  const renderer_t* r, shader_t shader, uint32_t tex, const mat4_t projection)
{
  glDisable(GL_DEPTH_TEST);
  program_t* progs = (program_t*)r->shaders;
  program_t* s = &progs[shader];

  glUniformMatrix4fv(s->locs[projection_loc], 1, GL_FALSE, projection);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(s->locs[tex_loc], 0);
}

void gl_setup_shader(const renderer_t* r, const draw_call_t* dc)
{
  program_t* progs = (program_t*)r->shaders;
  program_t* s = &progs[dc->shader];

  gl_set_active_shader(r, dc->shader);

  switch (dc->shader) {
    case shader_default:
      glEnable(GL_DEPTH_TEST);
      glUniformMatrix4fv(s->locs[transform_loc], 1, GL_FALSE, dc->transform);
      glUniformMatrix4fv(s->locs[view_loc], 1, GL_FALSE, r->view);
      glUniformMatrix4fv(
        s->locs[projection_loc], 1, GL_FALSE, dc->projection);

      gl_tex_bind(dc->tex, 0);
      glUniform1i(s->locs[tex_loc], 0);
      break;
    case shader_billboard:
      glEnable(GL_DEPTH_TEST);
      glUniformMatrix4fv(s->locs[transform_loc], 1, GL_FALSE, dc->transform);
      glUniformMatrix4fv(s->locs[view_loc], 1, GL_FALSE, dc->view);
      glUniformMatrix4fv(
        s->locs[projection_loc], 1, GL_FALSE, dc->projection);

      glUniform2f(s->locs[tex_size_loc], dc->tex->size.x, dc->tex->size.y);
      glUniform1f(s->locs[scale_loc], 0.005);

      gl_tex_bind(dc->tex, 0);
      glUniform1i(s->locs[tex_loc], 0);
      break;
    case shader_text:
    case shader_2d:
      gl_setup_2d(r, dc->shader, *(uint32_t*)dc->tex->handle, dc->projection);
      break;
    default:
      return;
  }
}
