#include "vertex_array.h"

vert_fmt_t vert_fmt_create(
  const vert_attr_t* attribs,
  size_t count)
{
  vert_fmt_t fmt;
  vert_fmt_update(&fmt, attribs, count);
  return fmt;
}

void vert_fmt_update(
  vert_fmt_t* fmt,
  const vert_attr_t* attribs,
  size_t count)
{
  fmt->attribs = attribs;
  fmt->attrib_count = count;

  size_t stride = 0;
  for (size_t i = 0; i < fmt->attrib_count; i++) {
    size_t size = get_data_type_size(fmt->attribs[i].type);
    stride += size * fmt->attribs[i].components;
  }

  fmt->stride = stride;
}

vert_arr_t* vert_arr_create(
  renderer_t* r,
  const vert_fmt_t* fmt)
{
  return r->backend.vertex_array_create(fmt);
}

void vert_arr_destroy(renderer_t* r, vert_arr_t* varr)
{
  return r->backend.vertex_array_destroy(varr);
}

void vert_arr_bind(renderer_t* r, vert_arr_t* varr)
{
  return r->backend.vertex_array_bind(varr);
}

void vert_arr_draw(
  renderer_t* r,
  vert_arr_t* varr,
  size_t start,
  size_t count,
  idx_mode_t index_mode)
{
  return r->backend.vertex_array_draw(varr, start, count, index_mode);
}

void vert_arr_draw_idx(
  renderer_t* r,
  vert_arr_t* varr,
  buf_obj_t* ebo,
  size_t count,
  data_type_t type,
  idx_mode_t index_mode)
{
  return r->backend.vertex_array_draw_indexed(
    varr,
    ebo,
    count,
    type,
    index_mode
  );
}
