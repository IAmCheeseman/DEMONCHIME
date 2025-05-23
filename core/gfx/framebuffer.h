#ifndef __engine_gfx_framebuffer__
#define __engine_gfx_framebuffer__

#include "include.h"
#include "math/vec2i.h"
#include "gfx_types.h"
#include "gfx.h"

typedef struct framebuf_s framebuf_t;

framebuf_t* framebuf_create(
  const renderer_t* r,
  vfs_t* vfs,
  vec2i_t size,
  uint8_t flags
);
void framebuf_destroy(const renderer_t* r, framebuf_t* fb);
void framebuf_bind(const renderer_t* r, const framebuf_t* fb);
void framebuf_resize(const renderer_t* r, framebuf_t* fb, vec2i_t size);
void framebuf_draw(
  const renderer_t* r, framebuf_t* fb, vec2i_t start, vec2i_t end);

#endif
