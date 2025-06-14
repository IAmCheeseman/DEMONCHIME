#include "gl_font.h"

#include <glad/glad.h>

#include "c_mem.h"
#include "c_color.h"
#include "m_math.h"
#include "gl_vert_arr.h"
#include "gl_buf_obj.h"
#include "gl_framebuf.h"

#define glyph_count 128

typedef struct glyph
{
  // uint32_t tex_handle;
  vec2f_t uv_start;
  vec2f_t uv_end;
  vec2i_t size;
  vec2i_t bearing;
  uint32_t advance;
} glyph_t;

typedef struct font_handle
{
  vec2i_t atlas_size;
  uint32_t atlas_handle;
  vert_arr_t* vao;
  buf_obj_t* vbo;
} font_handle_t;

void gl_font_init(renderer_t* r, font_t* font, SFT* sft)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glyph_t* glyphs = 
    (glyph_t*)mem_alloc(sizeof(glyph_t) * glyph_count);
  font->glyphs = glyphs;

  vec2i_t atlas_size = {0};

  // find the atlas size and collect some basic information about glyphs
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

    int width = (mtx.minWidth + 3) & ~3;
    int height = mtx.minHeight;

    atlas_size.x += width;
    atlas_size.y = fmax(atlas_size.y, height);

    glyph_t* glyph = &glyphs[c];
    glyph->size = (vec2i_t){
      width,
      height,
    };
    glyph->bearing = (vec2i_t){
      -mtx.leftSideBearing,
      -mtx.yOffset,
    };
    glyph->advance = mtx.advanceWidth;
  }

  uint32_t atlas;
  glGenTextures(1, &atlas);
  glBindTexture(GL_TEXTURE_2D, atlas);
  glTexImage2D(
    GL_TEXTURE_2D,
    0, GL_RED,
    atlas_size.x, atlas_size.y,
    0, GL_RED, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // populate the atlas
  vec2i_t gpos = {0};
  for (uint8_t c = 0; c < 128; c++)
  {
    glyph_t* glyph = &glyphs[c];

    SFT_Image img = (SFT_Image){
      .width = glyph->size.x,
      .height = glyph->size.y,
    };

    SFT_Glyph gid;
    if (sft_lookup(sft, c, &gid)) {
      log_warning("failed to load glyph ('%c')", c);
      continue;
    }

    char pixels[img.width * img.height];
    img.pixels = pixels;
    if (sft_render(sft, gid, img) < 0) {
      log_warning("could not render glyph ('%c')", c);
      continue;
    }

    glTexSubImage2D(
      GL_TEXTURE_2D,
      0,
      gpos.x, gpos.y,
      glyph->size.x, glyph->size.y,
      GL_RED, GL_UNSIGNED_BYTE,
      img.pixels);

    glyph->uv_start.x = (float)gpos.x / atlas_size.x;
    glyph->uv_start.y = (float)gpos.y / atlas_size.y;
    glyph->uv_end.x = (float)(gpos.x + glyph->size.x) / atlas_size.x;
    glyph->uv_end.y = (float)(gpos.y + glyph->size.y) / atlas_size.y;
    
    gpos.x += glyph->size.x;
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  font_handle_t* handle = (font_handle_t*)mem_alloc(sizeof(font_handle_t));
  font->handle = handle;

  handle->atlas_size = atlas_size;
  handle->atlas_handle = atlas;
  handle->vbo = gl_buf_obj_create(buf_arr);
  gl_buf_obj_bind(handle->vbo);
  handle->vao = gl_vert_arr_create(r, shader_text);

  log_debug(
    "loaded font (vao %d, vbo %d, atlas %d)",
    handle->vao->handle, handle->vbo->handle, handle->atlas_handle);

  gl_buf_obj_unbind(buf_arr);
  gl_vert_arr_bind(NULL);
}

void gl_font_destroy(font_t* font)
{
  font_handle_t* handle = (font_handle_t*)font->handle;

  handle->atlas_size = (vec2i_t){0, 0};
  glDeleteTextures(1, &handle->atlas_handle);

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
  renderer_t* r,
  font_t* font,
  vec2f_t pos,
  const char* text,
  color_t color)
{
  font_handle_t* handle = (font_handle_t*)font->handle;

  // gl_shader_bind(shader);
  // gl_shader_send_vec4f(shader, "text_color", (vec4f_t){
  //   color.r, color.g, color.b, color.a
  // });
  // gl_shader_send_mat4(shader, "p", r->projection);
  // gl_shader_send_int(shader, "fontmap", 0);

  // glActiveTexture(GL_TEXTURE0);

  float x = pos.x;
  float y = pos.y + font->max_height;

  int len = strlen(text);

  size_t verts_size = (r->shader_fmts[shader_text].stride * 6) * len;
  float* verts = (float*)mem_alloc(verts_size);
  int offset = 0;

  for (const char* c = text; *c != '\0'; c++) {
    glyph_t* g = &font->glyphs[(int)*c];

    float xx = x + g->bearing.x;
    float yy = y - g->bearing.y;
    float w = g->size.x;
    float h = g->size.y;

    float gverts[] = {
      xx,     yy + h, g->uv_start.x, g->uv_end.y,   color.r, color.g, color.b, color.a,
      xx,     yy,     g->uv_start.x, g->uv_start.y, color.r, color.g, color.b, color.a,
      xx + w, yy,     g->uv_end.x,   g->uv_start.y, color.r, color.g, color.b, color.a,

      xx,     yy + h, g->uv_start.x, g->uv_end.y,   color.r, color.g, color.b, color.a,
      xx + w, yy,     g->uv_end.x,   g->uv_start.y, color.r, color.g, color.b, color.a,
      xx + w, yy + h, g->uv_end.x,   g->uv_end.y,   color.r, color.g, color.b, color.a,
    };
    memcpy(verts + offset, gverts, sizeof(gverts));

    offset += 8 * 6;

    x += g->advance;
  }

  gl_set_active_shader(r, shader_text);
  gl_setup_2d(r, shader_text, handle->atlas_handle, r->projection_2d);

  // glBindBuffer(GL_ARRAY_BUFFER, handle->vbo->handle);
  // glBufferSubData(GL_ARRAY_BUFFER, 0, verts_size, verts);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);

  gl_buf_obj_set_dat(handle->vbo, verts, verts_size, draw_dynamic);
  mem_destroy(verts);

  gl_vert_arr_draw(r, handle->vao, 0, len * 6, idx_triangles);
}
