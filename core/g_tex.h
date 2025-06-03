#ifndef __engine_texture__
#define __engine_texture__

#include "include.h"
#include "g_renderer.h"
#include "g_types.h"
#include "c_img.h"
#include "m_math.h"

// TODO: make this consistent with other implementation-defined stuff
struct tex
{
  void* handle;
  vec2i_t size;
  img_fmt_t format;
  tex_filter_t min_filter;
  tex_filter_t mag_filter;
  tex_wrap_t x_wrap;
  tex_wrap_t y_wrap;
};

tex_t tex_load(const renderer_t* r, vfs_t* vfs, const char* path);
tex_t tex_load_from_img(const renderer_t* r, const img_t* img);
void tex_destroy(const renderer_t* r, tex_t* tex);
void tex_gen_mipmap(const renderer_t* r, tex_t* tex);
void tex_bind(const renderer_t* r, const tex_t* tex, uint8_t slot);
void tex_set_filter(
  const renderer_t* r,
  tex_t* tex,
  tex_filter_t min,
  tex_filter_t mag);
void tex_set_wrap(
  const renderer_t* r,
  tex_t* tex,
  tex_wrap_t x_wrap,
  tex_wrap_t y_wrap);

#endif
