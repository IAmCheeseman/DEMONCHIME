#ifndef __engine_gfx_framebuffer__
#define __engine_gfx_framebuffer__

#include "include.h"

#include "g_renderer.h"
#include "g_types.h"
#include "m_vec2i.h"

typedef struct framebuf framebuf_t;

framebuf_t* framebuf_create(const renderer_t* r, vec2i_t size, uint8_t flags);
void framebuf_destroy(const renderer_t* r, framebuf_t* fb);
void framebuf_bind(const renderer_t* r, const framebuf_t* fb);
void framebuf_resize(const renderer_t* r, framebuf_t* fb, vec2i_t size);
void framebuf_draw(
  renderer_t* r, framebuf_t* fb, vec2i_t start, vec2i_t end);

#endif
