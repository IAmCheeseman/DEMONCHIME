#include "gfx_text.h"

#include "gfx_shader.h"
#include "core_mem.h"

shader_t* text_shader = NULL;

void init_freetype(renderer_t* r, vfs_t* core_vfs)
{
  if (FT_Init_FreeType(&r->freetype)) {
    log_fatal(1, "could not init freetype");
  }

  text_shader = shader_load_from_files(
    r, core_vfs, "res/vtext.glsl", "res/ftext.glsl");
}

void destroy_freetype(renderer_t* r)
{
  FT_Done_FreeType(r->freetype);
  shader_destroy(r, text_shader);
}

font_t* font_load(
  const renderer_t* r, vfs_t* vfs, const char* ttf, int size)
{
  if (!text_shader)
    log_fatal(1, "can't load '%s' because freetype is not init", ttf);
  font_t* font = mem_alloc(sizeof(font_t));
  font->glyphs = NULL;
  font->max_height = size;

  size_t ttf_size;
  uint8_t* ttf_data = vfs_read(vfs, ttf, &ttf_size);

  FT_Face face;
  if (FT_New_Memory_Face(r->freetype, ttf_data, ttf_size, 0, &face))
    log_fatal(1, "could not load font '%s'", ttf);

  FT_Set_Pixel_Sizes(face, 0, size);

  r->backend.font_init(font, face);

  FT_Done_Face(face);
  mem_destroy(ttf_data);

  return font;
}

void font_destroy(const renderer_t* r, font_t* font)
{
  r->backend.font_destroy(font);
  mem_destroy(font);
}

int font_get_height(const renderer_t* r, const font_t* font)
{
  return font->max_height;
}

int font_get_width(const renderer_t* r, const font_t* font, const char* text)
{
  return r->backend.font_get_width(font, text);
}

void font_draw(
  const renderer_t* r, font_t* font, vec2f_t pos, const char* text)
{
  r->backend.font_draw(font, pos, text, (color_t){1, 1, 1, 1}, text_shader);
}

void font_draw_colored(
  const renderer_t* r,
  font_t* font,
  vec2f_t pos, color_t color,
  const char* text)
{
  r->backend.font_draw(font, pos, text, color, text_shader);
}

void font_draw_custom(
  const renderer_t* r,
  font_t* font,
  vec2f_t pos,
  color_t color,
  const char* text,
  shader_t* shader)
{
  r->backend.font_draw(font, pos, text, color, shader);
  mem_destroy(font);
}
