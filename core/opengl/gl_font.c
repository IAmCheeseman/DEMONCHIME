#include "gl_font.h"

#include <glad/glad.h>

#include "c_mem.h"
#include "c_color.h"
#include "m_math.h"
#include "gl_vert_arr.h"
#include "gl_buf_obj.h"

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
  vert_arr_t* vao;
  buf_obj_t* vbo;
} font_handle_t;

void gl_font_init(const renderer_t* r, font_t* font, SFT* sft)
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

  handle->vbo = gl_buf_obj_create(buf_arr);
  gl_buf_obj_set_dat(
    handle->vbo,
    NULL,
    r->shader_fmts[shader_text].stride * 6,
    draw_dynamic);

  handle->vao = gl_vert_arr_create(r, shader_text);

  log_debug(
    "loaded font (vao %d, vbo %d)",
    handle->vao->handle, handle->vbo->handle);

  gl_buf_obj_unbind(buf_arr);
  gl_vert_arr_bind(NULL);
}

void gl_font_destroy(font_t* font)
{
  for (int i = 0; i < glyph_count; i++) {
    glDeleteTextures(1, &font->glyphs[i].tex_handle);
  }

  font_handle_t* handle = (font_handle_t*)font->handle;

  gl_vert_arr_destroy(handle->vao);
  gl_buf_obj_destroy(handle->vbo);

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
  color_t color)
{
  font_handle_t* handle = (font_handle_t*)font->handle;

  gl_set_active_shader(r, shader_text);
  gl_setup_2d(r, shader_text, 0, r->projection_2d);

  // gl_shader_bind(shader);
  // gl_shader_send_vec4f(shader, "text_color", (vec4f_t){
  //   color.r, color.g, color.b, color.a
  // });
  // gl_shader_send_mat4(shader, "p", r->projection);
  // gl_shader_send_int(shader, "fontmap", 0);

  // glActiveTexture(GL_TEXTURE0);
  gl_vert_arr_bind(handle->vao);

  float x = pos.x;
  float y = pos.y + font->max_height;

  for (const char* c = text; *c != '\0'; c++) {
    glyph_t* g = &font->glyphs[(int)*c];

    float xx = x + g->bearing.x;
    float yy = y - g->bearing.y;
    float w = g->size.x;
    float h = g->size.y;

    float verts[] = {
      xx,     yy + h,  0, 1,  color.r, color.g, color.b, color.a,
      xx,     yy,      0, 0,  color.r, color.g, color.b, color.a,
      xx + w, yy,      1, 0,  color.r, color.g, color.b, color.a,

      xx,     yy + h,  0, 1,  color.r, color.g, color.b, color.a,
      xx + w, yy,      1, 0,  color.r, color.g, color.b, color.a,
      xx + w, yy + h,  1, 1,  color.r, color.g, color.b, color.a,
    };

    glBindTexture(GL_TEXTURE_2D, g->tex_handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle->vbo->handle);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    x += g->advance;
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
