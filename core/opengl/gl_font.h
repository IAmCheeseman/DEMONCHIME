#ifndef __engine_gfx_opengl_gl_text__
#define __engine_gfx_opengl_gl_text__

#include <schrift/schrift.h>

#include "include.h"
#include "g_font.h"
#include "gl_shader.h"

void gl_font_init(const renderer_t* r, font_t* font, SFT* sft);
void gl_font_destroy(font_t* font);
int gl_font_get_width(const font_t* font, const char* text);
void gl_font_draw(
  const renderer_t* r,
  font_t* font,
  vec2f_t pos,
  const char* text,
  color_t color);

#endif
