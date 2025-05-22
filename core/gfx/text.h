#ifndef __engine_gfx_text__
#define __engine_gfx_text__

#include <ft2build.h>
#include FT_FREETYPE_H

#include "include.h"
#include "buffer_object.h"
#include "vertex_array.h"

#include "gfx.h"

typedef struct font_s
{
  void* handle;
  struct glyph_s* glyphs;
} font_t;

void init_freetype(renderer_t* r, vfs_t* core_vfs);
void destroy_freetype(renderer_t* r);

font_t* font_create(renderer_t* r, vfs_t* vfs, const char* ttf, int size);
void font_destroy(renderer_t* r, font_t* font);
void font_draw(renderer_t* r, font_t* font, vec2f_t pos, const char* text);

#endif
