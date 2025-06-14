#ifndef __engine_gfx_opengl_gl_framebuffer__
#define __engine_gfx_opengl_gl_framebuffer__

#include "include.h"
#include "g_framebuf.h"
#include "g_vert_arr.h"
#include "g_buf_obj.h"

struct framebuf
{
  vec2i_t size;
  uint8_t flags;
  uint32_t fb_handle;
  uint32_t color_handle;
  // will either be a depth buffer, or a depth24-stencil8 buffer
  uint32_t z_mask_handle;

  // to draw the framebuffer as a quad
  vert_arr_t* vao;
  buf_obj_t* vbo;
};

framebuf_t* gl_framebuf_create(
  const renderer_t* r,
  vec2i_t size,
  uint8_t flags);
void gl_framebuf_destroy(framebuf_t* fb);
void gl_framebuf_bind(const framebuf_t* fb);
void gl_framebuf_resize(framebuf_t* fb, vec2i_t size);
void gl_framebuf_draw(
  renderer_t* r, framebuf_t* fb, vec2i_t start, vec2i_t end);

#endif
