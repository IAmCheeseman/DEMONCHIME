#ifndef __engine_texture__
#define __engine_texture__

#include "include.h"
#include "gfx_types.h"
#include "gfx.h"
#include "image.h"
#include "math/vec2i.h"

// TODO: make this consistent with other implementation-defined stuff
typedef struct tex_s
{
  void* handle;
  vec2i_t size;
  img_fmt_t format;
  tex_filter_t min_filter;
  tex_filter_t mag_filter;
  tex_wrap_t x_wrap;
  tex_wrap_t y_wrap;
} tex_t;

tex_t tex_load(renderer_t* r, vfs_t* vfs, const char* path);
tex_t tex_load_from_img(renderer_t* r, img_t* img);
void tex_destroy(renderer_t* r, tex_t* tex);
void tex_gen_mipmap(renderer_t* r, tex_t* tex);
void tex_bind(renderer_t* r, tex_t* tex, uint8_t slot);
void tex_set_filter(
  renderer_t* r,
  tex_t* tex,
  tex_filter_t min,
  tex_filter_t mag);
void tex_set_wrap(
  renderer_t* r,
  tex_t* tex,
  tex_wrap_t x_wrap,
  tex_wrap_t y_wrap);

#endif
