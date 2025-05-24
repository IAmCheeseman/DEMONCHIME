#ifndef __engine_gfx_gfx__
#define __engine_gfx_gfx__

#include <ft2build.h>
#include FT_FREETYPE_H

#include "gfx_backend.h"
#include "core_engine.h"
#include "math_vec2f.h"
#include "math_vec3f.h"
#include "math_mat4.h"

typedef enum gfx_backend_type_e
{
  gfx_backend_opengl,
} gfx_backend_type_t;

// might put more stuff here later
typedef struct renderer_s
{
  FT_Library freetype;
  gfx_backend_t backend;
} renderer_t;

void init_backend(engine_t* engine, gfx_backend_type_t backend);
void clear_bg(const renderer_t* renderer, float r, float g, float b);
void adjust_viewport(const renderer_t* r, vec2f_t size);
void set_depth_test(const renderer_t* r, bool test);

#endif
