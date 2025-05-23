#ifndef __engine_wrap__
#define __engine_wrap__

#include <luajit/lua.h>
#include <luajit/lauxlib.h>
#include <luajit/lualib.h>

#include "include.h"
#include "engine.h"
#include "math/mat4.h"

typedef enum ldata_type_e
{
  lua_type_invalid = 0,
  lua_type_engine,
  lua_type_img,
  lua_type_tex,
  lua_type_mesh,
  lua_type_vert_fmt,
  lua_type_shader,
  lua_type_font,
  lua_type_mat4,
  lua_type_prng,
} ldata_type_t;

#define core_name "core"

#define engine_mt_name "__ENGINE_MT__"
#define img_mt_name "__IMG_MT__"
#define tex_mt_name "__TEX_MT__"
#define mesh_mt_name "__MESH_MT__"
#define vert_fmt_mt_name "__VERT_FMT_MT__"
#define shader_mt_name "__SHADER_MT__"
#define mat4_mt_name "__MAT4_MT__"
#define font_mt_name "__FONT_MT__"
#define prng_mt_name "__PRNG_MT__"

typedef struct lmat4_s
{
  mat4_t m;
} lmat4_t;

typedef struct ldata_s
{
  ldata_type_t type;
  void* ptr;
} ldata_t;

void create_ldata(
  lua_State* L, void* data, const char* mt, ldata_type_t type);
void* read_ldata(lua_State* L, int index, ldata_type_t expected);
engine_t* get_engine(lua_State* L);
void reg_funcs(lua_State* L, const luaL_Reg* funcs);
bool protected_do_file(lua_State* L, vfs_t* vfs, const char* file);
void raw_insert(lua_State* L, int t, int k, int pos);

void wrap(lua_State* L, engine_t* engine);
void wrap_engine(lua_State* L);
void wrap_enums(lua_State* L);
void wrap_tex(lua_State* L);
void wrap_log(lua_State* L);
void wrap_shader(lua_State* L);
void wrap_vert_fmt(lua_State* L);
void wrap_mesh(lua_State* L);
void wrap_mat4(lua_State* L);
void wrap_font(lua_State* L);
void wrap_prng(lua_State* L);

#endif
