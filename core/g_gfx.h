#ifndef __engine_gfx_gfx__
#define __engine_gfx_gfx__

#include "g_backend.h"
#include "c_engine.h"
#include "m_math.h"

typedef enum gfx_backend_type_e
{
  gfx_backend_opengl,
} gfx_backend_type_t;

// might put more stuff here later
typedef struct renderer_s
{
  mat4_t projection;
  gfx_backend_t backend;
} renderer_t;

void init_backend(engine_t* engine, gfx_backend_type_t backend);
void set_projection(renderer_t* renderer, mat4_t projection);
void clear_bg(const renderer_t* renderer, float r, float g, float b);
void adjust_viewport(const renderer_t* r, vec2f_t size);
void set_depth_test(const renderer_t* r, bool test);

#endif
