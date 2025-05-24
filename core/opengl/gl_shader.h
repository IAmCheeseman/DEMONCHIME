#ifndef __engine_gfx_opengl_gl_shader__
#define __engine_gfx_opengl_gl_shader__

#include "g_shader.h"

struct shader_s
{
  uint32_t handle;
  shader_tab_t uniforms;
  shader_tab_t attrs;
};

shader_t* gl_shader_load_from_files(
  vfs_t* vfs,
  const char* vert,
  const char* frag
);
shader_t* gl_shader_load_from_src(const char* vert, const char* frag);
void gl_shader_send_int(shader_t* s, const char* name, int i);
void gl_shader_send_float(shader_t* s, const char* name, float f);
void gl_shader_send_vec2f(shader_t* s, const char* name, vec2f_t v);
void gl_shader_send_vec2i(shader_t* s, const char* name, vec2i_t v);
void gl_shader_send_vec3f(shader_t* s, const char* name, vec3f_t v);
void gl_shader_send_vec3i(shader_t* s, const char* name, vec3i_t v);
void gl_shader_send_vec4f(shader_t* s, const char* name, vec4f_t v);
void gl_shader_send_vec4i(shader_t* s, const char* name, vec4i_t v);
void gl_shader_send_mat4(shader_t* s, const char* name, const mat4_t m);
void gl_shader_bind(const shader_t* s);
void gl_shader_destroy(shader_t* s);

#endif
