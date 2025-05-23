#include "gl_text.h"

#include <glad/glad.h>

#include "math/vec2i.h"
#include "mem.h"
#include "color.h"

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

void gl_font_init(font_t* font, FT_Face face)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glyph_t* glyphs = 
    (glyph_t*)mem_alloc(sizeof(glyph_t) * glyph_count);
  font->glyphs = glyphs;

  for (uint8_t c = 0; c < 128; c++)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      log_warning("failed to load glyph for '%c'", c);
      continue;
    }

    uint32_t tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(
      GL_TEXTURE_2D,
      0, GL_RED,
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      0,
      GL_RED, GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glyph_t* glyph = &glyphs[c];
    glyph->tex_handle = tex;
    glyph->size = (vec2i_t){
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows
    };
    glyph->bearing = (vec2i_t){
        face->glyph->bitmap_left,
        face->glyph->bitmap_top
    };
    glyph->advance = face->glyph->advance.x;
  }

  font_handle_t* handle = (font_handle_t*)mem_alloc(sizeof(font_handle_t));
  font->handle = handle;

  glGenVertexArrays(1, &handle->vao);
  glGenBuffers(1, &handle->vbo);
  glBindVertexArray(handle->vao);
  glBindBuffer(GL_ARRAY_BUFFER, handle->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

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

void gl_font_draw(
  font_t* font,
  vec2f_t pos,
  const char* text,
  color_t color,
  shader_t* shader)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  font_handle_t* handle = (font_handle_t*)font->handle;

  mat4_t p;
  Mat4Ortho(p, 0, 320 * 5, 180 * 5, 0, 0, 100);

  gl_shader_bind(shader);
  gl_shader_send_vec4f(shader, "text_color", (vec4f_t){
    color.r, color.g, color.b, color.a
  });
  gl_shader_send_mat4(shader, "p", p);
  gl_shader_send_int(shader, "fontmap", 0);

  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(handle->vao);

  float x = pos.x;
  float y = pos.y + 48;

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

    x += g->advance >> 6;
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
