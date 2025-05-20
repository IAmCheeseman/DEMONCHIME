#include "wrap.h"

#include "mem.h"

static int L_err_handler(lua_State* L)
{
  lua_getglobal(L, "debug");
  if (!lua_istable(L, -1)) {
    log_fatal(
      1, "%s\ndebug library has been removed; cannot provide stack trace",
      lua_tostring(L, 1));
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_iscfunction(L, -1)) {
    log_fatal(
      1, "%s\n'debug.traceback' has been removed; cannot provide stack trace",
      lua_tostring(L, 1));
  }

  lua_pushvalue(L, 1);
  lua_pushnumber(L, 0);
  lua_call(L, 2, 1);

  if (lua_isstring(L, -1)) {
    log_error("%s", lua_tostring(L, -1));
  } else {
    log_error("%s\n(stack trace unavailable)", lua_tostring(L, 1));
  }
  return 0;
}

static const char* ldata_type_to_str(ldata_type_t type)
{
  switch (type) {
    case LUA_TYPE_INVALID: return "Invalid";
    case LUA_TYPE_ENGINE: return "Engine";
    case LUA_TYPE_IMAGE: return "Image";
    case LUA_TYPE_TEXTURE: return "Texture";
    case LUA_TYPE_MESH: return "Mesh";
    case LUA_TYPE_VERTEX_FORMAT: return "VertexFormat";
    case LUA_TYPE_SHADER: return "Shader";
    case LUA_TYPE_MAT4: return "Mat4";
    case LUA_TYPE_PRNG: return "Prng";
  }
  return "Invalid";
}

static void type_err(lua_State* L, int index, ldata_type_t expected)
{
  const char* type_str = ldata_type_to_str(expected);
  luaL_error(L, "expected type '%s' for argument #%d", type_str, index);
}

void create_ldata(
  lua_State* L,
  void* data,
  const char* mt,
  ldata_type_t type)
{
  ldata_t* ld =
    (ldata_t*)lua_newuserdata(L, sizeof(ldata_t));
  ld->type = type;
  ld->ptr = data;

  luaL_getmetatable(L, mt);
  lua_setmetatable(L, -2);
}

void* read_ldata(lua_State* L, int index, ldata_type_t expected)
{
  if (!lua_isuserdata(L, index)) {
    type_err(L, index, expected);
  }

  ldata_t* data = (ldata_t*)lua_touserdata(L, index);

  if (data->type != expected) {
    type_err(L, index, expected);
  }

  if (data->ptr == NULL) {
    luaL_error(L, "cannot use freed userdata");
  }

  return data->ptr;
}

void reg_funcs(lua_State* L, const luaL_Reg* funcs)
{
  for (const luaL_Reg* func = funcs; func->name != NULL; func++) {
    lua_pushcfunction(L, func->func);
    lua_setfield(L, -2, func->name);
  }
}

bool protected_do_file(lua_State* L, engine_t* engine, const char* file)
{
  size_t src_len;
  char* src = vfs_read_txt(engine->vfs, file, &src_len);
  if (!src) {
    log_warning("cannot execute '%s'", file);
    return false;
  }
  int status = luaL_loadbufferx(L, src, src_len, file, NULL);
  mem_destroy(src);

  if (status != LUA_OK) {
    lua_pushcfunction(L, L_err_handler);
    lua_pushvalue(L, -2);
    lua_call(L, 1, 0);
    return false;
  }
  
  status = lua_pcall(L, 0, 0, 0);
  if (status != LUA_OK) {
    lua_pushcfunction(L, L_err_handler);
    lua_pushvalue(L, -2);
    lua_call(L, 1, 0);
    return false;
  }

  return true;
}

void raw_insert(lua_State* L, int t, int v, int pos)
{
  if (t < 0) t = lua_gettop(L) + 1 + t;
  if (v < 0) v = lua_gettop(L) + 1 + v;

  if (pos == -1) {
    lua_pushvalue(L, v);
    lua_rawseti(L, t, lua_objlen(L, t) + 1);
    return;
  } else {
    pos = lua_objlen(L, t) + 1 + pos;
  }

  for (int i = lua_objlen(L, t) + 1; i > pos; i--) {
    lua_rawgeti(L, t, i - 1);
    lua_rawseti(L, t, i);
  }

  lua_pushvalue(L, v);
  lua_rawseti(L, t, pos);
}

void wrap(lua_State* L, engine_t* engine)
{
  engine->L = L;

  lua_pushcfunction(L, L_err_handler);
  engine->lua_err_handler_idx = lua_gettop(L);

  lua_newtable(L);
  lua_setglobal(L, CORE_NAME);

  wrap_engine(L);
  wrap_enums(L);
  wrap_tex(L);
  wrap_log(L);
  wrap_shader(L);
  wrap_vert_fmt(L);
  wrap_mesh(L);
  wrap_mat4(L);
  wrap_prng(L);

  luaL_newmetatable(L, ENGINE_MT_NAME);

  create_ldata(L, engine, ENGINE_MT_NAME, LUA_TYPE_ENGINE);
  lua_setfield(L, LUA_REGISTRYINDEX, "__BSE_ENGINE__");
}

engine_t* get_engine(lua_State* L)
{
  lua_getfield(L, LUA_REGISTRYINDEX, "__BSE_ENGINE__");
  engine_t* engine = (engine_t*)read_ldata(L, -1, LUA_TYPE_ENGINE);
  lua_pop(L, 1);
  return engine;
}
