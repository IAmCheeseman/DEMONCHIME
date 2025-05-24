#include "core_image.h"

#include "stb_image.h"
#include "core_mem.h"

static img_fmt_t img_fmt_from_channel_count(int channels)
{
  switch (channels) {
    case 1: return img_fmt_r8;
    case 2: return img_fmt_ra8;
    case 3: return img_fmt_rgb8;
    case 4: return img_fmt_rgba8;
  }
  return img_fmt_invalid;
}

img_t image_load(vfs_t* vfs, const char* path)
{
  vec2i_t size;
  int channel_count;
  size_t file_dat_len;
  uint8_t* file_dat = (uint8_t*)vfs_read(vfs, path, &file_dat_len);
  if (!file_dat) log_fatal(1, "could not load texture '%s'", path);
  uint8_t* data = stbi_load_from_memory(
    file_dat, file_dat_len,
    &size.x, &size.y, &channel_count, 0);
  mem_destroy(file_dat);

  img_fmt_t format = img_fmt_from_channel_count(channel_count);

  log_debug("loading image '%s'", path);

  return image_load_from_mem(data, size, format);
}

img_t image_load_from_mem(uint8_t* data, vec2i_t size, img_fmt_t format)
{
  img_t img;
  img.data = data;
  img.size = size;
  img.format = format;
  return img;
}

void image_destroy(img_t* img)
{
  mem_destroy(img->data);
  img->data = NULL;
  img->size = (vec2i_t){0, 0};
  img->format = img_fmt_invalid;
}
