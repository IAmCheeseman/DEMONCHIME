#include "w_wrap.h"

#include <GLFW/glfw3.h>

#include "m_math.h"
#include "c_mem.h"
#include "g_renderer.h"

static int L_get_total_time(lua_State* L)
{
  lua_pushnumber(L, glfwGetTime());
  return 1;
}

static int L_get_fps(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lua_pushnumber(L, engine->timer.fps);
  return 1;
}

static int L_get_tps(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lua_pushnumber(L, engine->timer.tps);
  return 1;
}

static int L_get_dt(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lua_pushnumber(L, engine->timer.dt);
  return 1;
}

static int L_set_tick_rate(lua_State* L)
{
  engine_t* engine = get_engine(L);
  engine->timer.tick_rate = luaL_checkinteger(L, 1);
  return 1;
}

static int L_get_tick_rate(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lua_pushnumber(L, engine->timer.tick_rate);
  return 1;
}

static int L_get_tick_rate_sec(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lua_pushnumber(L, 1.0 / engine->timer.tick_rate);
  return 1;
}

static int L_close_engine(lua_State* L)
{
  engine_t* engine = get_engine(L);
  engine_close(engine);
  return 0;
}

static int L_set_fullscreen(lua_State* L)
{
  engine_t* engine = get_engine(L);
  fullscreen_t fullscreen = (fullscreen_t)luaL_checkinteger(L, 1);
  window_set_fullscreen(&engine->window, fullscreen);
  return 0;
}

static int L_get_window_size(lua_State* L)
{
  engine_t* engine = get_engine(L);
  vec2i_t s = window_get_size(&engine->window);
  lua_pushinteger(L, s.x);
  lua_pushinteger(L, s.y);
  return 2;
}

static int L_get_screen_size(lua_State* L)
{
  engine_t* engine = get_engine(L);
  vec2i_t s = engine_get_screen_size(engine);
  lua_pushinteger(L, s.x);
  lua_pushinteger(L, s.y);
  return 2;
}

static int L_get_interpolation(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lua_pushnumber(L, engine->timer.accum / (1.0 / engine->timer.tick_rate));
  return 1;
}

static int L_interpolate(lua_State* L)
{
  engine_t* engine = get_engine(L);
  double a = luaL_checknumber(L, 1);
  double b = luaL_checknumber(L, 2);
  lua_pushnumber(
    L,
    (b - a) * (engine->timer.accum / (1.0 / engine->timer.tick_rate)) + a
  );
  return 1;
}

static int L_set_projection(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lmat4_t* m = (lmat4_t*)read_ldata(L, 1, lua_type_mat4);
  memcpy(engine->renderer->projection_3d, m->m, sizeof(mat4_t));
  return 0;
}

static int L_get_projection(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lmat4_t* m = (lmat4_t*)mem_alloc(sizeof(lmat4_t));
  memcpy(m->m, engine->renderer->projection_3d, sizeof(mat4_t));
  create_ldata(L, m, mat4_mt_name, lua_type_mat4);
  return 1;
}

static int L_set_2d_projection(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lmat4_t* m = (lmat4_t*)read_ldata(L, 1, lua_type_mat4);
  memcpy(engine->renderer->projection_2d, m->m, sizeof(mat4_t));
  return 0;
}

static int L_get_2d_projection(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lmat4_t* m = (lmat4_t*)mem_alloc(sizeof(lmat4_t));
  memcpy(m->m, engine->renderer->projection_2d, sizeof(mat4_t));
  create_ldata(L, m, mat4_mt_name, lua_type_mat4);
  return 1;
}

static int L_set_view(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lmat4_t* m = (lmat4_t*)read_ldata(L, 1, lua_type_mat4);
  memcpy(engine->renderer->view, m->m, sizeof(mat4_t));
  return 0;
}

static int L_get_view(lua_State* L)
{
  engine_t* engine = get_engine(L);
  lmat4_t* m = (lmat4_t*)mem_alloc(sizeof(lmat4_t));
  memcpy(m->m, engine->renderer->view, sizeof(mat4_t));
  create_ldata(L, m, mat4_mt_name, lua_type_mat4);
  return 1;
}

static int L_is_key_down(lua_State* L)
{
  engine_t* engine = get_engine(L);
  int key = luaL_checkinteger(L, 1);
  lua_pushboolean(L, is_key_down(engine, (keys_t)key));
  return 1;
}

static int L_is_mouse_down(lua_State* L)
{
  engine_t* engine = get_engine(L);
  int btn = luaL_checkinteger(L, 1);
  lua_pushboolean(L, is_mouse_down(engine, btn));
  return 1;
}

static int L_get_mouse_pos(lua_State* L)
{
  engine_t* engine = get_engine(L);
  vec2f_t pos = get_mouse_pos(engine);
  lua_pushnumber(L, pos.x);
  lua_pushnumber(L, pos.y);
  return 2;
} 

luaL_Reg engine_funcs[] = {
  {"get_total_time", L_get_total_time},
  {"get_fps", L_get_fps},
  {"get_dt", L_get_dt},
  {"get_tps", L_get_tps},
  {"set_tick_rate", L_set_tick_rate},
  {"get_tick_rate", L_get_tick_rate},
  {"get_tick_rate_sec", L_get_tick_rate_sec},
  {"close_engine", L_close_engine},
  {"set_fullscreen", L_set_fullscreen},
  {"get_window_size", L_get_window_size},
  {"get_screen_size", L_get_screen_size},
  {"get_interpolation", L_get_interpolation},
  {"interpolate", L_interpolate},
  {"set_projection", L_set_projection},
  {"get_projection", L_get_projection},
  {"set_2d_projection", L_set_2d_projection},
  {"get_2d_projection", L_get_2d_projection},
  {"set_view", L_set_view},
  {"get_view", L_get_view},
  {"is_key_down", L_is_key_down},
  {"is_mouse_down", L_is_mouse_down},
  {"get_mouse_pos", L_get_mouse_pos},
  {NULL, NULL},
};

static int L_loader(lua_State* L)
{
  engine_t* engine = get_engine(L);

  size_t module_len;
  const char* module = luaL_checklstring(L, 1, &module_len);
  char* module_cpy = (char*)mem_alloc(sizeof(char) * (module_len + 1));
  memcpy(module_cpy, module, module_len);
  module_cpy[module_len] = '\0';

  for (char* c = module_cpy; *c != '\0'; c++)
    if (*c == '.') *c = '/';

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
    for (const char* c = cur_path; *c != ';' && *c != '\0'; c++)
      cur_path_len++;
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
    char* path = (char*)mem_alloc(sizeof(char) * (path_len + 1));
    if (q_pos > 0)
      memcpy(path, cur_path, q_pos); // copy everything before ?
    memcpy(path + q_pos, module_cpy, module_len); // copy module len
    memcpy(
      path + q_pos + module_len,
      cur_path + q_pos + 1,
      cur_path_len - q_pos - 1);
    path[path_len] = '\0';

    // check if file exists
    if (vfs_exists(engine->vfs, path)) {
      // it does; load
      size_t src_len;
      char* src = vfs_read_txt(engine->vfs, path, &src_len);
      if (src == NULL) {
        mem_destroy(path);
        mem_destroy(module_cpy);
        return luaL_error(L, "error loading file");
      }

      int status = luaL_loadbuffer(L, src, src_len, path);

      switch (status) {
        case LUA_ERRMEM:
          return luaL_error(L, "memory error: %s\n", lua_tostring(L, -1));
        case LUA_ERRSYNTAX:
          return luaL_error(L, "syntax error: %s\n", lua_tostring(L, -1));
      }

      mem_destroy(src);
      mem_destroy(path);
      mem_destroy(module_cpy);
      return 1;
    }

    mem_destroy(path);
    cur_path += cur_path_len + 1;
  }

  mem_destroy(module_cpy);

  lua_pushfstring(L, "\n\tno file '%s' in default vfs", module);
  return 1;
}

void wrap_engine(lua_State* L)
{
  lua_getglobal(L, core_name);
  reg_funcs(L, engine_funcs);
  lua_pushstring(L, bse_os_str);
  lua_setfield(L, -2, "os");
  lua_pop(L, 1);

  // register loader
  lua_getglobal(L, "package"); // cannot be nil, nothing has yet been loaded
  lua_getfield(L, -1, "loaders"); // ditto
  lua_pushcfunction(L, L_loader);
  lua_rawseti(L, -2, 2); // TODO: maybe actually insert someday
  // LuaRawInsert(L, -2, -1, 2);
  lua_pop(L, 2);
}
