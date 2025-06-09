#ifndef __engine_gfx_opengl_texture__
#define __engine_gfx_opengl_texture__

#include "g_tex.h"

tex_t gl_tex_load_from_img(const img_t* img);
tex_t gl_tex_1x1_color(color_t color);
void gl_tex_destroy(tex_t* tex);
void gl_tex_bind(const tex_t* tex, uint8_t slot);
void gl_tex_gen_mipmap(tex_t* tex);
void gl_tex_set_filter(tex_t* tex, tex_filter_t min, tex_filter_t mag);
void gl_tex_set_wrap(tex_t* tex, tex_wrap_t x_wrap, tex_wrap_t y_wrap);

#endif
