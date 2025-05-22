#ifndef __engine_gfx_backend__
#define __engine_gfx_backend__

#include <ft2build.h>
#include FT_FREETYPE_H

#include "include.h"

#include "gfx_types.h"
#include "math/vec2f.h"
#include "math/vec3f.h"
#include "math/vec4f.h"
#include "math/vec2i.h"
#include "math/vec3i.h"
#include "math/vec4i.h"
#include "math/mat4.h"

struct engine_s;
struct renderer_s;
struct buf_obj_s;
struct vert_arr_s;
struct vert_fmt_s;
struct img_s;
struct tex_s;
struct shader_s;
struct framebuf_s;
struct font_s;
struct vfs_s;
struct color_s;

typedef void (*clear_bg_fn)(float r, float g, float b);
typedef void (*adjust_viewport_fn)(vec2f_t size);
typedef void (*set_depth_test_fn)(bool do_test);

typedef struct buf_obj_s* (*buf_obj_create_fn)(
  buf_obj_type_t type
);
typedef void (*buf_obj_bind_fn)(struct buf_obj_s* buf);
typedef void (*buf_obj_unbind_fn)(buf_obj_type_t type);
typedef void (*buf_obj_set_dat_fn)(
  struct buf_obj_s* buf,
  void* data,
  size_t size,
  draw_mode_t mode
);
typedef void (*buf_obj_destroy_fn)(struct buf_obj_s* buf);

typedef struct vert_arr_s* (*vert_arr_create_fn)(
  const struct vert_fmt_s* fmt);
typedef void (*vert_arr_destroy_fn)(struct vert_arr_s* varr);
typedef void (*vert_arr_bind_fn)(struct vert_arr_s* varr);
typedef void (*vert_arr_draw_fn)(
  struct vert_arr_s* varr,
  size_t start,
  size_t count,
  idx_mode_t index_mode
);
typedef void (*vert_arr_draw_idx_fn)(
  struct vert_arr_s* varr,
  struct buf_obj_s* ebo,
  size_t count,
  data_type_t type,
  idx_mode_t index_mode
);

typedef struct framebuf_s* (*framebuf_create_fn)(
  struct vfs_s* vfs,
  vec2i_t size,
  uint8_t flags
);
typedef void (*framebuf_destroy_fn)(struct framebuf_s* fb);
typedef void (*framebuf_bind_fn)(struct framebuf_s* fb);
typedef void (*framebuf_resize_fn)(struct framebuf_s* fb, vec2i_t size);
typedef void (*framebuf_draw_fn)(
  struct framebuf_s* fb,
  vec2i_t start,
  vec2i_t end
);

typedef struct shader_s* (*shader_load_from_files_fn)(
  struct vfs_s* vfs,
  const char* vert,
  const char* frag
);
typedef struct shader_s* (*shader_load_from_src_fn)(
  const char* vert,
  const char* frag
);
typedef void (*shader_send_int_fn)(
  struct shader_s* s,
  const char* name,
  int i
);
typedef void (*shader_send_float_fn)(
  struct shader_s* s,
  const char* name,
  float f
);
typedef void (*shader_send_vec2f_fn)(
  struct shader_s* s,
  const char* name,
  vec2f_t v
);
typedef void (*shader_send_vec2i_fn)(
  struct shader_s* s,
  const char* name,
  vec2i_t v
);
typedef void (*shader_send_vec3f_fn)(
  struct shader_s* s,
  const char* name, 
  vec3f_t v
);
typedef void (*shader_send_vec3i_fn)(
  struct shader_s* s,
  const char* name,
  vec3i_t v
);
typedef void (*shader_send_vec4f_fn)(
  struct shader_s* s,
  const char* name,
  vec4f_t v
);
typedef void (*shader_send_vec4i_fn)(
  struct shader_s* s,
  const char* name,
  vec4i_t v
);
typedef void (*shader_send_mat4_fn)(struct shader_s* s, const char* name, mat4_t m);
typedef void (*shader_bind_fn)(struct shader_s* s);
typedef void (*shader_destroy_fn)(struct shader_s* s);

typedef struct tex_s (*tex_load_from_img_fn)(struct img_s* img);
typedef void (*tex_gen_mipmap_fn)(struct tex_s* tex);
typedef void (*tex_bind_fn)(struct tex_s* tex, uint8_t slot);
typedef void (*tex_set_filter_fn)(
  struct tex_s* tex,
  tex_filter_t min,
  tex_filter_t mag
);
typedef void (*tex_set_wrap_fn)(
  struct tex_s* tex,
  tex_wrap_t x_wrap,
  tex_wrap_t y_wrap
);
typedef void (*tex_destroy_fn)(struct tex_s* tex);

typedef void (*font_init_fn)(
  struct renderer_s* r, struct font_s* font, FT_Face face);
typedef void (*font_destroy_fn)(struct renderer_s* r, struct font_s* font);
typedef void (*font_draw_fn)(
  struct font_s* font,
  vec2f_t pos,
  const char* text,
  struct color_s color, 
  struct shader_s* shader);

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

  shader_load_from_files_fn shader_load_files;
  shader_load_from_src_fn shader_load_src;
  shader_destroy_fn shader_destroy;;
  shader_send_int_fn shader_send_int;
  shader_send_float_fn shader_send_float;
  shader_send_vec2f_fn shader_send_vec2f;
  shader_send_vec2i_fn shader_send_vec2i;
  shader_send_vec3f_fn shader_send_vec3f;
  shader_send_vec3i_fn shader_send_vec3i;
  shader_send_vec4f_fn shader_send_vec4f;
  shader_send_vec4i_fn shader_send_vec4i;
  shader_send_mat4_fn shader_send_mat4;
  shader_bind_fn shader_bind;

  tex_load_from_img_fn texture_load_img;
  tex_destroy_fn texture_destroy;
  tex_gen_mipmap_fn texture_gen_mipmaps;
  tex_bind_fn texture_bind;
  tex_set_filter_fn texture_set_filter;
  tex_set_wrap_fn texture_set_wrap;

  font_init_fn font_init;
  font_destroy_fn font_destroy;
  font_draw_fn font_draw;
} gfx_backend_t;

#endif
