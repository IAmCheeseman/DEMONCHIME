#include "wrap.h"

#include <GLFW/glfw3.h>

#include "math/vec2i.h"
#include "mem.h"

static int L_GetTotalTime(lua_State* L)
{
  lua_pushnumber(L, glfwGetTime());
  return 1;
}

static int L_GetFps(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  lua_pushnumber(L, engine->timer.fps);
  return 1;
}

static int L_GetTps(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  lua_pushnumber(L, engine->timer.tps);
  return 1;
}

static int L_CloseEngine(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  EngineClose(engine);
  return 0;
}

static int L_GetWindowSize(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  vec2i_t s = EngineGetWindowSize(engine);
  lua_pushinteger(L, s.x);
  lua_pushinteger(L, s.y);
  return 2;
}

static int L_GetScreenSize(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  vec2i_t s = EngineGetScreenSize(engine);
  lua_pushinteger(L, s.x);
  lua_pushinteger(L, s.y);
  return 2;
}

static int L_GetInterpolation(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  lua_pushnumber(L, engine->timer.accum / (1.0 / engine->timer.tick_rate));
  return 1;
}

static int L_Interpolate(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  lua_pushnumber(
    L,
    (b - a) * (engine->timer.accum / (1.0 / engine->timer.tick_rate)) + a
  );
  return 1;
}

static int L_IsKeyDown(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  int key = luaL_checkinteger(L, 1);
  lua_pushboolean(L, IsKeyDown(engine, (enum Key)key));
  return 1;
}

static int L_IsMouseDown(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  int btn = luaL_checkinteger(L, 1);
  lua_pushboolean(L, IsMouseDown(engine, btn));
  return 1;
}

static int L_GetMousePosition(lua_State* L)
{
  struct Engine* engine = GetEngine(L);
  vec2f_t pos = GetMousePosition(engine);
  lua_pushnumber(L, pos.x);
  lua_pushnumber(L, pos.y);
  return 2;
} 

luaL_Reg engine_funcs[] = {
  {"GetTotalTime", L_GetTotalTime},
  {"GetFps", L_GetFps},
  {"GetTps", L_GetTps},
  {"CloseEngine", L_CloseEngine},
  {"GetWindowSize", L_GetWindowSize},
  {"GetScreenSize", L_GetScreenSize},
  {"GetInterpolation", L_GetInterpolation},
  {"Interpolate", L_Interpolate},
  {"IsKeyDown", L_IsKeyDown},
  {"IsMouseDown", L_IsMouseDown},
  {"GetMousePosition", L_GetMousePosition},
  {NULL, NULL},
};

static int L_Loader(lua_State* L)
{
  struct Engine* engine = GetEngine(L);

  size_t module_len;
  const char* module = luaL_checklstring(L, 1, &module_len);
  char* module_cpy = CreateArray(char, module_len + 1);
  memcpy(module_cpy, module, module_len);
  module_cpy[module_len] = '\0';

  for (char* c = module_cpy; *c != '\0'; c++) {
    if (*c == '.') *c = '/';
  }

  lua_getglobal(L, "package");
  if (lua_isnil(L, -1))
    return luaL_error(L, "'package' removed, cannot load files");

  lua_getfield(L, -1, "path");
  if (!lua_isstring(L, -1))
    return luaL_error(L, "'package.path' removed, cannot load files");

  size_t paths_len;
  const char* paths = lua_tolstring(L, -1, &paths_len);
  lua_pop(L, 2);

  const char* cur_path = paths;

  while (cur_path < paths + paths_len) {
    size_t cur_path_len = 0;
    for (const char* c = cur_path; *c != ';' && *c != '\0'; c++) {
      cur_path_len++;
    }
    if (cur_path_len == 0) break;

    size_t q_pos = SIZE_MAX;
    for (size_t j = 0; j < cur_path_len; j++) {
      if (cur_path[j] == '?') {
        q_pos = j;
        break;
      }
    }

    // invalid path
    if (q_pos == SIZE_MAX) continue;

    // -1 because ? is removed
    size_t path_len = cur_path_len + module_len - 1;
    char* path = CreateArray(char, path_len + 1);
    if (q_pos > 0)
      memcpy(path, cur_path, q_pos); // copy everything before ?
    memcpy(path + q_pos, module_cpy, module_len); // copy module len
    memcpy(path + q_pos + module_len, cur_path + q_pos + 1, cur_path_len - q_pos - 1);
    path[path_len] = '\0';

    // check if file exists
    if (VfsDoesFileExist(engine->vfs, path)) {
      // it does; load
      size_t src_len;
      char* src = VfsReadTxtFile(engine->vfs, path, &src_len);
      if (src == NULL)
        return luaL_error(L, "error loading file");

      int status = luaL_loadbuffer(L, src, src_len, path);

      switch (status) {
        case LUA_ERRMEM:
          return luaL_error(L, "memory error: %s\n", lua_tostring(L, -1));
        case LUA_ERRSYNTAX:
          return luaL_error(L, "syntax error: %s\n", lua_tostring(L, -1));
      }

      Destroy(src);
      Destroy(path);
      Destroy(module_cpy);
      return 1;
    }

    Destroy(path);
    cur_path += cur_path_len + 1;
  }

  Destroy(module_cpy);

  lua_pushfstring(L, "\n\tno file '%s' in default vfs", module);
  return 1;
}

void WrapEngine(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  RegisterFunctions(L, engine_funcs);
  lua_pushstring(L, bse_os_str);
  lua_setfield(L, -2, "os");
  lua_pop(L, 1);

  // register loader
  lua_getglobal(L, "package"); // cannot be nil, nothing has yet been loaded
  lua_getfield(L, -1, "loaders"); // ditto
  lua_pushcfunction(L, L_Loader);
  lua_rawseti(L, -2, 2); // TODO: maybe actually insert someday
  // LuaRawInsert(L, -2, -1, 2);
  lua_pop(L, 3);
}
