#ifndef __engine_opengl_gl_shader__
#define __engine_opengl_gl_shader__

#include "include.h"

#include "g_renderer.h"

void gl_init_shaders(renderer_t* r, vfs_t* vfs);
void gl_destroy_shaders(renderer_t* r);
void gl_set_active_shader(const renderer_t* r, uint8_t shader);
void gl_setup_2d(
  const renderer_t* r, shader_t shader, uint32_t tex, const mat4_t projection);
void gl_setup_shader(const renderer_t* r, const draw_call_t* dc);

#endif
