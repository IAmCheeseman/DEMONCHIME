#include "g_shader.h"

void init_shaders(renderer_t* r, vfs_t* vfs)
{
  r->backend.init_shaders(r, vfs);
  set_active_shader(r, shader_default);
}

shader_t get_active_shader(const renderer_t* r)
{
  return r->active_shader;
}

void set_active_shader(renderer_t* r, shader_t shader)
{
  r->backend.set_active_shader(r, shader);
  r->active_shader = shader;
}

void setup_shader(const renderer_t* r, const draw_call_t* dc)
{
  r->backend.setup_shader(r, dc);
}
