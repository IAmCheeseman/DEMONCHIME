#ifndef __engine_gfx_gfx__
#define __engine_gfx_gfx__

#include <ft2build.h>
#include FT_FREETYPE_H

#include "engine.h"
#include "math/vec2f.h"
#include "math/vec3f.h"
#include "math/mat4.h"
#include "backend.h"

typedef enum gfx_backend_type_e
{
  GFX_BACKEND_OPENGL,
} gfx_backend_type_t;

// might put more stuff here later
typedef struct renderer_s
{
  FT_Library freetype;
  gfx_backend_t backend;
} renderer_t;

void init_backend(engine_t* engine, gfx_backend_type_t backend);
void clear_bg(renderer_t* renderer, float r, float g, float b);
void adjust_viewport(renderer_t* r, vec2f_t size);
void set_depth_test(renderer_t* r, bool test);

#endif
