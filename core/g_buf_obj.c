#include "g_buf_obj.h"

buf_obj_t* buf_obj_create(
  const renderer_t* r,
  buf_obj_type_t type)
{
  return r->backend.buf_obj_create(type);
}

void buf_obj_destroy(const renderer_t* r, buf_obj_t* buf)
{
  r->backend.buf_obj_destroy(buf);
}

void buf_obj_bind(const renderer_t* r, const buf_obj_t* buf)
{
  r->backend.buf_obj_bind(buf);
}

void buf_obj_unbind(const renderer_t* r, buf_obj_type_t type)
{
  r->backend.buf_obj_unbind(type);
}

void buf_obj_set_dat(
  const renderer_t* r,
  buf_obj_t* buf,
  void* data,
  size_t size,
  draw_mode_t mode)
{
  r->backend.buf_obj_set(buf, data, size, mode);
}
