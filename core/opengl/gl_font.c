#include "gl_font.h"

#include <glad/glad.h>

#include "c_mem.h"
#include "c_color.h"
#include "m_math.h"

#define glyph_count 128

typedef struct glyph_s
{
  uint32_t tex_handle;
  vec2i_t size;
  vec2i_t bearing;
  uint32_t advance;
} glyph_t;

typedef struct font_handle_s
{
  uint32_t vao;
  uint32_t vbo;
} font_handle_t;

void gl_font_init(font_t* font, SFT* sft)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glyph_t* glyphs = 
    (glyph_t*)mem_alloc(sizeof(glyph_t) * glyph_count);
  font->glyphs = glyphs;

  for (uint8_t c = 0; c < 128; c++)
  {
    SFT_Glyph gid;
    if (sft_lookup(sft, c, &gid)) {
      log_warning("failed to load glyph ('%c')", c);
      continue;
    }

    SFT_GMetrics mtx;
    if (sft_gmetrics(sft, gid, &mtx)) {
      log_warning("bad glyph metrics ('%c')", c);
      continue;
    }

    SFT_Image img = (SFT_Image){
      .width = (mtx.minWidth + 3) & ~3,
      .height = mtx.minHeight,
    };
    char pixels[img.width * img.height];
    img.pixels = pixels;
    if (sft_render(sft, gid, img) < 0) {
      log_warning("could not render glyph ('%c')", c);
      continue;
    }

    uint32_t tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(
      GL_TEXTURE_2D,
      0, GL_RED,
      img.width,
      img.height,
      0,
      GL_RED, GL_UNSIGNED_BYTE,
      img.pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glyph_t* glyph = &glyphs[c];
    glyph->tex_handle = tex;
    glyph->size = (vec2i_t){
      img.width,
      img.height,
    };
    glyph->bearing = (vec2i_t){
      -mtx.leftSideBearing,
      -mtx.yOffset,
    };
    glyph->advance = mtx.advanceWidth;
  }

  font_handle_t* handle = (font_handle_t*)mem_alloc(sizeof(font_handle_t));
  font->handle = handle;

  glGenVertexArrays(1, &handle->vao);
  glGenBuffers(1, &handle->vbo);
  glBindVertexArray(handle->vao);
  glBindBuffer(GL_ARRAY_BUFFER, handle->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
    1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

  log_debug(
    "loaded font (vao %d, vbo %d)", handle->vao, handle->vbo);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void gl_font_destroy(font_t* font)
{
  for (int i = 0; i < glyph_count; i++) {
    glDeleteTextures(1, &font->glyphs[i].tex_handle);
  }

  font_handle_t* handle = (font_handle_t*)font->handle;

  glDeleteVertexArrays(1, &handle->vao);
  glDeleteBuffers(1, &handle->vbo);

  mem_destroy(font->handle);
  mem_destroy(font->glyphs);

  log_debug("destroyed font");
}

int gl_font_get_width(const font_t* font, const char* text)
{
  int width = 0;
  for (const char* c = text; *c != '\0'; c++) {
    glyph_t* g = &font->glyphs[(int)*c];
    width += g->advance;
  }
  return width;
}

void gl_font_draw(
  const renderer_t* r,
  font_t* font,
  vec2f_t pos,
  const char* text,
  color_t color,
  shader_t* shader)
{
  font_handle_t* handle = (font_handle_t*)font->handle;

  mat4_t p;
  mat4_ortho(p, 0, 320 * 5, 180 * 5, 0, 0, 100);

  gl_shader_bind(shader);
  gl_shader_send_vec4f(shader, "text_color", (vec4f_t){
    color.r, color.g, color.b, color.a
  });
  gl_shader_send_mat4(shader, "p", r->projection);
  gl_shader_send_int(shader, "fontmap", 0);

  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(handle->vao);

  float x = pos.x;
  float y = pos.y + font->max_height;

  for (const char* c = text; *c != '\0'; c++) {
    glyph_t* g = &font->glyphs[(int)*c];

    float xx = x + g->bearing.x;
    float yy = y - g->bearing.y;
    float w = g->size.x;
    float h = g->size.y;

    float verts[] = {
      xx,     yy + h, 0, 1,
      xx,     yy,     0, 0,
      xx + w, yy,     1, 0,

      xx,     yy + h, 0, 1,
      xx + w, yy,     1, 0,
      xx + w, yy + h, 1, 1,
    };
    
    glBindTexture(GL_TEXTURE_2D, g->tex_handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    x += g->advance;
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
