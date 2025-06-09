#include "g_tex.h"

tex_t tex_load(
  const renderer_t* r,
  vfs_t* vfs,
  const char* path)
{
  img_t img = image_load(vfs, path);
  tex_t tex = tex_load_from_img(r, &img);
  image_destroy(&img);
  return tex;
}

tex_t tex_1x1_color(const renderer_t* r, color_t color)
{
  return r->backend.tex_1x1_color(color);
}

tex_t tex_load_from_img(const renderer_t* r, const img_t* img)
{
  return r->backend.tex_load_img(img);
}

void tex_destroy(const renderer_t* r, tex_t* tex)
{
  r->backend.tex_destroy(tex);
}

void tex_gen_mipmap(const renderer_t* r, tex_t* tex)
{
  r->backend.tex_gen_mipmaps(tex);
}

void tex_bind(const renderer_t* r, const tex_t* tex, uint8_t slot)
{
  r->backend.tex_bind(tex, slot);
}

void tex_set_filter(
  const renderer_t* r,
  tex_t* tex,
  tex_filter_t min,
  tex_filter_t mag)
{
  r->backend.tex_set_filter(tex, min, mag);
}

void tex_set_wrap(
  const renderer_t* r,
  tex_t* tex,
  tex_wrap_t x_wrap,
  tex_wrap_t y_wrap)
{
  r->backend.tex_set_wrap(tex, x_wrap, y_wrap);
}
