#ifndef __engine_wrap_wrap_vertex_format__
#define __engine_wrap_wrap_vertex_format__

#include "include.h"
#include "wrap.h"
#include "gfx/vertex_array.h"

typedef struct lvertex_format_s
{
  vert_attr_t* attribs;
  size_t count;
  size_t capacity;
  vert_fmt_t fmt;
} lvertex_format_t;

#endif
