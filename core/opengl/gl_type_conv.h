#ifndef __engine_gfx_opengl_gl_type_conv__
#define __engine_gfx_opengl_gl_type_conv__

#include "include.h"
#include "gfx_types.h"

uint32_t buf_obj_type_to_gl(buf_obj_type_t type);
uint32_t draw_mode_to_gl(draw_mode_t mode);
uint32_t data_type_to_gl(data_type_t type);
uint32_t idx_mode_to_gl(idx_mode_t mode);
uint32_t img_fmt_to_gl(img_fmt_t format);
uint32_t tex_filter_to_gl(tex_filter_t filter);
uint32_t tex_wrap_to_gl(tex_wrap_t wrap);

#endif
