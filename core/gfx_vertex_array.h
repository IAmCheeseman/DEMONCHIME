#ifndef __engine_gfx_vertex_array__
#define __engine_gfx_vertex_array__

#include "include.h"
#include "gfx.h"
#include "gfx_types.h"
#include "gfx_buffer_object.h"

typedef struct vert_attr_s
{
  data_type_t type : 8;
  uint8_t components; // is it a vec2, vec3, or whatever?
} vert_attr_t;

typedef struct vert_fmt_s
{
  const vert_attr_t* attribs;
  size_t attrib_count;
  size_t stride;
} vert_fmt_t;

typedef struct vert_arr_s vert_arr_t;

vert_fmt_t vert_fmt_create(const vert_attr_t* attribs, size_t count);
void vert_fmt_update(vert_fmt_t* fmt, const vert_attr_t* attribs, size_t count);

vert_arr_t* vert_arr_create(const renderer_t* r, const vert_fmt_t* fmt);
void vert_arr_destroy(const renderer_t* r, vert_arr_t* varr);
void vert_arr_bind(const renderer_t* r, const vert_arr_t* varr);
void vert_arr_draw(
  const renderer_t* r,
  const vert_arr_t* varr,
  size_t start,
  size_t count,
  idx_mode_t index_mode);
void vert_arr_draw_idx(
  const renderer_t* r,
  const vert_arr_t* varr,
  const buf_obj_t* ebo,
  size_t count,
  data_type_t type,
  idx_mode_t index_mode);

#endif
