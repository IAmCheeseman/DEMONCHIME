#ifndef __engine_g_shader__
#define __engine_g_shader__

#include "g_renderer.h"

void init_shaders(renderer_t* r, vfs_t* vfs);
shader_t get_active_shader(const renderer_t* r);
void set_active_shader(renderer_t* r, shader_t shader);
void setup_shader(const renderer_t* r, const draw_call_t* dc);

#endif
