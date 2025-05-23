#ifndef __engine_gfx_opengl_gl_text__
#define __engine_gfx_opengl_gl_text__

#include "include.h"
#include "gfx/text.h"
#include "gl_shader.h"

void gl_font_init(font_t* font, FT_Face face);
void gl_font_destroy(font_t* font);
void gl_font_draw(
  font_t* font,
  vec2f_t pos,
  const char* text,
  color_t color,
  shader_t* shader);

#endif
