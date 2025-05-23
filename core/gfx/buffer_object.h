#ifndef __engine_gfx_buffer_object__
#define __engine_gfx_buffer_object__

#include "include.h"
#include "gfx_types.h"
#include "gfx.h"

typedef struct buf_obj_s buf_obj_t;

buf_obj_t* buf_obj_create(
  const renderer_t* r,
  buf_obj_type_t type
);
void buf_obj_destroy(const renderer_t* r, buf_obj_t* buf);
void buf_obj_bind(const renderer_t* r, const buf_obj_t* buf);
void buf_obj_unbind(const renderer_t* r, buf_obj_type_t type);
void buf_obj_set_dat(
  const renderer_t* r,
  buf_obj_t* buf,
  void* data,
  size_t size,
  draw_mode_t mode
);

#endif
