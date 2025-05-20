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
  LUA_TYPE_INVALID = 0,
  LUA_TYPE_ENGINE,
  LUA_TYPE_IMG,
  LUA_TYPE_TEX,
  LUA_TYPE_MESH,
  LUA_TYPE_VERT_FMT,
  LUA_TYPE_SHADER,
  LUA_TYPE_MAT4,
  LUA_TYPE_PRNG,
} ldata_type_t;

#define CORE_NAME "core"

#define ENGINE_MT_NAME "__ENGINE_MT__"
#define IMG_MT_NAME "__IMG_MT__"
#define TEX_MT_NAME "__TEX_MT__"
#define MESH_MT_NAME "__MESH_MT__"
#define VERT_FMT_MT_NAME "__VERT_FMT_MT__"
#define SHADER_MT_NAME "__SHADER_MT__"
#define MAT4_MT_NAME "__MAT4_MT__"
#define PRNG_MT_NAME "__PRNG_MT__"

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
void wrap_prng(lua_State* L);

#endif
