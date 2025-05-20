#include "framebuffer.h"

framebuf_t* framebuf_create(
  renderer_t* r,
  vfs_t* vfs,
  vec2i_t size,
  uint8_t flags)
{
  return r->backend.framebuffer_create(vfs, size, flags);
}

void framebuf_destroy(renderer_t* r, framebuf_t* fb)
{
  return r->backend.framebuffer_destroy(fb);
}

void framebuf_bind(renderer_t* r, framebuf_t* fb)
{
  return r->backend.framebuffer_bind(fb);
}

void framebuf_resize(
  renderer_t* r,
  framebuf_t* fb,
  vec2i_t size)
{
  return r->backend.framebuffer_resize(fb, size);
}

void framebuf_draw(
  renderer_t* r,
  framebuf_t* fb,
  vec2i_t start,
  vec2i_t end)
{
  return r->backend.framebuffer_draw(fb, start, end);
}
