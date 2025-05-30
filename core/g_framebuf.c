#include "g_framebuf.h"

framebuf_t* framebuf_create(
  const renderer_t* r,
  vfs_t* vfs,
  vec2i_t size,
  uint8_t flags)
{
  return r->backend.framebuf_create(r, vfs, size, flags);
}

void framebuf_destroy(const renderer_t* r, framebuf_t* fb)
{
  return r->backend.framebuf_destroy(fb);
}

void framebuf_bind(const renderer_t* r, const framebuf_t* fb)
{
  return r->backend.framebuf_bind(fb);
}

void framebuf_resize(
  const renderer_t* r,
  framebuf_t* fb,
  vec2i_t size)
{
  return r->backend.framebuf_resize(fb, size);
}

void framebuf_draw(
  const renderer_t* r,
  framebuf_t* fb,
  vec2i_t start,
  vec2i_t end)
{
  return r->backend.framebuf_draw(r, fb, start, end);
}
