#ifndef __engine_gfx_backend__
#define __engine_gfx_backend__

#include <schrift/schrift.h>

#include "include.h"

#include "g_types.h"
#include "m_math.h"

struct engine;
struct renderer;
struct draw_call;
struct buf_obj;
struct vert_arr;
struct vert_fmt;
struct img;
struct tex;
struct framebuf;
struct font;
struct vfs;
struct color;

typedef void (*clear_bg_fn)(float r, float g, float b);
typedef void (*adjust_viewport_fn)(vec2f_t size);
typedef void (*set_depth_test_fn)(bool do_test);

typedef struct buf_obj* (*buf_obj_create_fn)(buf_obj_type_t type);
typedef void (*buf_obj_bind_fn)(const struct buf_obj* buf);
typedef void (*buf_obj_unbind_fn)(buf_obj_type_t type);
typedef void (*buf_obj_set_dat_fn)(
  struct buf_obj* buf,
  void* data,
  size_t size,
  draw_mode_t mode);
typedef void (*buf_obj_destroy_fn)(struct buf_obj* buf);

typedef struct vert_arr* (*vert_arr_create_fn)(
  const struct renderer* r, shader_t shader);
typedef void (*vert_arr_destroy_fn)(struct vert_arr* varr);
typedef void (*vert_arr_bind_fn)(const struct vert_arr* varr);
typedef void (*vert_arr_draw_fn)(
  const struct vert_arr* varr,
  size_t start,
  size_t count,
  idx_mode_t index_mode);
typedef void (*vert_arr_draw_idx_fn)(
  const struct vert_arr* varr,
  const struct buf_obj* ebo,
  size_t count,
  data_type_t type,
  idx_mode_t index_mode);

typedef struct framebuf* (*framebuf_create_fn)(
  const struct renderer* r,
  struct vfs* vfs,
  vec2i_t size,
  uint8_t flags);
typedef void (*framebuf_destroy_fn)(struct framebuf* fb);
typedef void (*framebuf_bind_fn)(const struct framebuf* fb);
typedef void (*framebuf_resize_fn)(struct framebuf* fb, vec2i_t size);
typedef void (*framebuf_draw_fn)(
  const struct renderer* r,
  struct framebuf* fb,
  vec2i_t start,
  vec2i_t end);

typedef void (*init_shaders_fn)(struct renderer* r, struct vfs* vfs);
typedef void (*destroy_shaders_fn)(struct renderer* r);
typedef void (*set_active_shader_fn)(
  const struct renderer* r, uint8_t shader);
typedef void (*setup_shader_fn)(
  const struct renderer* r, const struct draw_call* dc);

typedef struct tex (*tex_load_from_img_fn)(const struct img* img);
typedef struct tex (*tex_1x1_color_fn)(struct color color);
typedef void (*tex_gen_mipmap_fn)(struct tex* tex);
typedef void (*tex_bind_fn)(const struct tex* tex, uint8_t slot);
typedef void (*tex_set_filter_fn)(
  struct tex* tex,
  tex_filter_t min,
  tex_filter_t mag
);
typedef void (*tex_set_wrap_fn)(
  struct tex* tex,
  tex_wrap_t x_wrap,
  tex_wrap_t y_wrap
);
typedef void (*tex_destroy_fn)(struct tex* tex);

typedef void (*font_init_fn)(
    const struct renderer* r, struct font* font, SFT* sft);
typedef int (*font_get_width_fn)(const struct font* font, const char* text);
typedef void (*font_destroy_fn)(struct font* font);
typedef void (*font_draw_fn)(
  const struct renderer* r,
  struct font* font,
  vec2f_t pos,
  const char* text,
  struct color color);

typedef struct gfx_backend_s
{
  clear_bg_fn clear_background;
  adjust_viewport_fn adjust_viewport;
  set_depth_test_fn set_depth_test;

  buf_obj_create_fn buf_obj_create;
  buf_obj_destroy_fn buf_obj_destroy;
  buf_obj_bind_fn buf_obj_bind;
  buf_obj_unbind_fn buf_obj_unbind;
  buf_obj_set_dat_fn buf_obj_set;

  vert_arr_create_fn vert_arr_create;
  vert_arr_destroy_fn vert_arr_destroy;
  vert_arr_bind_fn vert_arr_bind;
  vert_arr_draw_fn vert_arr_draw;
  vert_arr_draw_idx_fn vert_arr_draw_idx;

  framebuf_create_fn framebuf_create;
  framebuf_destroy_fn framebuf_destroy;
  framebuf_bind_fn framebuf_bind;
  framebuf_resize_fn framebuf_resize;
  framebuf_draw_fn framebuf_draw;

  init_shaders_fn init_shaders;
  destroy_shaders_fn destroy_shaders;
  set_active_shader_fn set_active_shader;
  setup_shader_fn setup_shader;

  tex_load_from_img_fn tex_load_img;
  tex_1x1_color_fn tex_1x1_color;
  tex_destroy_fn tex_destroy;
  tex_gen_mipmap_fn tex_gen_mipmaps;
  tex_bind_fn tex_bind;
  tex_set_filter_fn tex_set_filter;
  tex_set_wrap_fn tex_set_wrap;

  font_init_fn font_init;
  font_destroy_fn font_destroy;
  font_get_width_fn font_get_width;
  font_draw_fn font_draw;
} gfx_backend_t;

#endif
