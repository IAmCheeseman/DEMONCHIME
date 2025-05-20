#ifndef __engine_gfx_framebuffer__
#define __engine_gfx_framebuffer__

#include "include.h"
#include "math/vec2i.h"
#include "gfx_types.h"
#include "gfx.h"

typedef struct framebuf_s framebuf_t;

framebuf_t* framebuf_create(
  renderer_t* r,
  vfs_t* vfs,
  vec2i_t size,
  uint8_t flags
);
void framebuf_destroy(renderer_t* r, framebuf_t* fb);
void framebuf_bind(renderer_t* r, framebuf_t* fb);
void framebuf_resize(
  renderer_t* r,
  framebuf_t* fb,
  vec2i_t size
);
void framebuf_draw(
  renderer_t* r,
  framebuf_t* fb,
  vec2i_t start,
  vec2i_t end
);

#endif
