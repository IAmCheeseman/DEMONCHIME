#include "w_wrap.h"

#include <string.h>

#include "g_types.h"
#include "c_key.h"

typedef struct lenum_s
{
  const char* name;
  int value;
} lenum_t;

#define shader_name "shader"
#define image_format_name "img_format"
#define texture_filter_name "tex_filter"
#define texture_wrap_name "tex_wrap"
#define fullscreen_wrap_name "fullscreen"
#define key_name "key"

lenum_t shader[] = {
  {"default", shader_default},
  {"billboard", shader_billboard},
  {"_2d", shader_2d},
  {NULL, 0},
};

lenum_t image_format[] = {
  {"r8", img_fmt_r8},
  {"ra8", img_fmt_ra8},
  {"rgb8", img_fmt_rgb8},
  {"rgba8", img_fmt_rgba8},
  {NULL, 0},
};

lenum_t texture_filter[] = {
  {"nearest", tex_filter_nearest},
  {"nearest_mipmap", tex_filter_nearest_mipmap},
  {"nearest_mipmap_nearest", tex_filter_nearest_mipmap_nearest},
  {"linear", tex_filter_linear},
  {"linear_mipmap", tex_filter_linear_mipmap},
  {"linear_mipmap_nearest", tex_filter_linear_mipmap_nearest},
  {NULL, 0},
};

lenum_t texture_wrap[] = {
  {"repeat", tex_wrap_rep},
  {"mirrored_repeat", tex_wrap_mirror_rep},
  {"clamp_edge", tex_wrap_clamp_edge},
  {"clamp_border", tex_wrap_clamp_border},
  {NULL, 0},
};

lenum_t fullscreen_wrap[] = {
  {"none", fullscreen_none},
  {"exclusive", fullscreen_exclusive},
  {"borderless", fullscreen_borderless},
  {NULL, 0},
};

lenum_t key_wrap[] = {
#define key_def(name, _) {#name, key_##name},
#include "key_def.h"
#undef key_def
  {NULL, 0},
};

static void reg_enum(
  lua_State* L,
  const char* name,
  lenum_t* lenums)
{
  lua_newtable(L);

  for (lenum_t* enum_ = lenums; enum_->name != NULL; enum_++) {
    lua_pushinteger(L, enum_->value);
    lua_setfield(L, -2, enum_->name);
  }

  lua_setfield(L, -2, name);
}

void wrap_enums(lua_State* L)
{
  lua_getglobal(L, core_name);

  reg_enum(L, shader_name, shader);
  reg_enum(L, image_format_name, image_format);
  reg_enum(L, texture_filter_name, texture_filter);
  reg_enum(L, texture_wrap_name, texture_wrap);
  reg_enum(L, fullscreen_wrap_name, fullscreen_wrap);
  reg_enum(L, key_name, key_wrap);

  lua_pop(L, 1);
}
