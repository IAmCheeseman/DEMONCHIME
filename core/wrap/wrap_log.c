#include "wrap.h"

#include "log.h"

static void cat_args(lua_State* L, int start)
{
  int argc = lua_gettop(L);

  for (int i = start; i <= argc; i++) {
    lua_getglobal(L, "tostring");
    lua_pushvalue(L, i);
    lua_call(L, 1, 1);

    if (i != argc) {
      lua_pushstring(L, " ");
    }
  }
  lua_concat(L, (argc - start + 1) * 2 - 1);
}

static int L_log_info(lua_State* L)
{
  cat_args(L, 1);
  log_info("%s", lua_tostring(L, -1));
  return 0;
}

static int L_log_warning(lua_State* L)
{
  cat_args(L, 1);
  log_warning("%s", lua_tostring(L, -1));
  return 0;
}

static int L_log_error(lua_State* L)
{
  cat_args(L, 1);
  log_error("%s", lua_tostring(L, -1));
  return 0;
}

static int L_log_fatal(lua_State* L)
{
  cat_args(L, 2);
  log_fatal(luaL_checkinteger(L, 1), "%s", lua_tostring(L, -1));
  return 0;
}

luaL_Reg log_funcs[] = {
  {"log_info", L_log_info},
  {"log_warning", L_log_warning},
  {"log_error", L_log_error},
  {"log_fatal", L_log_fatal},
  {NULL, NULL},
};

void wrap_log(lua_State* L)
{
  lua_getglobal(L, "_G");
  reg_funcs(L, log_funcs);
  lua_pop(L, 1);
}
