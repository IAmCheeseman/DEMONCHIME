#include "w_wrap.h"

#include <string.h>

#include "g_types.h"
#include "c_key.h"

typedef struct lenum
{
  const char* name;
  int value;
} lenum_t;

lenum_t shader[] = {
  {"default", shader_default},
  {"billboard", shader_billboard},
  {"2d", shader_2d},
  {"text", shader_text},
  {NULL, 0},
};

lenum_t draw_mode[] = {
  {"static", draw_static},
  {"dynamic", draw_dynamic},
  {"stream", draw_stream},
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
    
    lua_pushstring(L, enum_->name);
    lua_rawseti(L, -2, enum_->value);
  }

  lua_setfield(L, LUA_REGISTRYINDEX, name);
}

const char* get_str_from_enum(lua_State* L, int e, const char* enum_table)
{
  lua_getfield(L, LUA_REGISTRYINDEX, enum_table);
  lua_rawgeti(L, -1, e);
  const char* s = luaL_checkstring(L, -1);
  lua_pop(L, 2);
  return s;
}

int get_enum_from_str(
    lua_State* L, const char* enum_name, const char* enum_table)
{
  lua_getfield(L, LUA_REGISTRYINDEX, enum_table);
  lua_getfield(L, -1, enum_name);
  int e = lua_tonumber(L, -1);
  lua_pop(L, 2);
  return e;
}

void wrap_enums(lua_State* L)
{
  lua_getglobal(L, core_name);

  reg_enum(L, shader_enum, shader);
  reg_enum(L, draw_mode_enum, draw_mode);
  reg_enum(L, image_fmt_enum, image_format);
  reg_enum(L, tex_filter_enum, texture_filter);
  reg_enum(L, tex_wrap_enum, texture_wrap);
  reg_enum(L, fullscreen_enum, fullscreen_wrap);
  reg_enum(L, key_enum, key_wrap);

  lua_pop(L, 1);
}
