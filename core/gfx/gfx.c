#include "gfx.h"

#ifdef bse_allow_opengl
#include "opengl/gl_gfx.h"
#endif

void init_backend(engine_t* engine, gfx_backend_type_t backend)
{
  switch (backend) {
    case GFX_BACKEND_OPENGL:
    #ifdef bse_allow_opengl
      gl_init_backend(engine);
    #else
      log_fatal(1, "opengl is not supported on this system");
    #endif
      break;
    default:
      log_fatal(1, "invalid graphics backend");
      break;
  }
}

void clear_bg(renderer_t* renderer, float r, float g, float b)
{
  return renderer->backend.clear_background(r, g, b);
}

void adjust_viewport(renderer_t* r, vec2f_t size)
{
  return r->backend.adjust_viewport(size);
}

void set_depth_test(renderer_t* r, bool test)
{
  return r->backend.set_depth_test(test);
}

