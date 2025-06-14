#ifndef __engine_gfx_text__
#define __engine_gfx_text__

#include <schrift/schrift.h>

#include "include.h"

#include "g_renderer.h"
#include "g_buf_obj.h"
#include "g_vert_arr.h"
#include "g_shader.h"
#include "c_color.h"

typedef struct font
{
  void* handle;
  int max_height;
  struct glyph* glyphs;
} font_t;

void init_freetype(renderer_t* r, vfs_t* core_vfs);
void destroy_freetype(renderer_t* r);

font_t* font_load(renderer_t* r, vfs_t* vfs, const char* ttf, int size);
void font_destroy(const renderer_t* r, font_t* font);
int font_get_height(const renderer_t* r, const font_t* font);
int font_get_width(const renderer_t* r, const font_t* font, const char* text);
void font_draw(renderer_t* r, font_t* font, vec2f_t pos, const char* text);
void font_draw_colored(
  renderer_t* r,
  font_t* font,
  vec2f_t pos,
  color_t color,
  const char* text);

#endif
