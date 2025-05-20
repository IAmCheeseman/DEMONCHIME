#include "buffer_object.h"

buf_obj_t* buf_obj_create(
  renderer_t* r,
  buf_obj_type_t type)
{
  return r->backend.buf_obj_create(type);
}

void buf_obj_destroy(renderer_t* r, buf_obj_t* buf)
{
  return r->backend.buf_obj_destroy(buf);
}

void buf_obj_bind(renderer_t* r, buf_obj_t* buf)
{
  return r->backend.buf_obj_bind(buf);
}

void buf_obj_unbind(renderer_t* r, buf_obj_type_t type)
{
  return r->backend.buf_obj_unbind(type);
}

void buf_obj_set_dat(
  renderer_t* r,
  buf_obj_t* buf,
  void* data,
  size_t size,
  draw_mode_t mode)
{
  return r->backend.buf_obj_set(buf, data, size, mode);
}
