#include "texture.h"

tex_t tex_load(
  renderer_t* r,
  vfs_t* vfs,
  const char* path)
{
  img_t img = image_load(vfs, path);
  tex_t tex = tex_load_from_img(r, &img);
  image_destroy(&img);
  return tex;
}

tex_t tex_load_from_img(renderer_t* r, img_t* img)
{
  return r->backend.texture_load_img(img);
}

void tex_destroy(renderer_t* r, tex_t* tex)
{
  return r->backend.texture_destroy(tex);
}

void tex_gen_mipmap(renderer_t* r, tex_t* tex)
{
  return r->backend.texture_gen_mipmaps(tex);
}

void tex_bind(renderer_t* r, tex_t* tex, uint8_t slot)
{
  return r->backend.texture_bind(tex, slot);
}

void tex_set_filter(
  renderer_t* r,
  tex_t* tex,
  tex_filter_t min,
  tex_filter_t mag)
{
  return r->backend.texture_set_filter(tex, min, mag);
}

void tex_set_wrap(
  renderer_t* r,
  tex_t* tex,
  tex_wrap_t x_wrap,
  tex_wrap_t y_wrap)
{
  return r->backend.texture_set_wrap(tex, x_wrap, y_wrap);
}
