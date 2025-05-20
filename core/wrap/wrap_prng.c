#include "wrap.h"

#include "prng.h"
#include "mem.h"

static int L_create_prng(lua_State* L)
{
  Prng* p = (Prng*)mem_alloc(sizeof(Prng));
  *p = prng_create(luaL_optinteger(L, 1, 0));
  create_ldata(L, p, PRNG_MT_NAME, LUA_TYPE_PRNG);
  return 1;
}

luaL_Reg prng_funcs[] = {
  {"create_prng", L_create_prng},
  {NULL, NULL},
};

static int L_PrngMt__index(lua_State* L)
{
  luaL_getmetatable(L, PRNG_MT_NAME);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_PrngMt_nexti(lua_State* L)
{
  Prng* p = (Prng*)read_ldata(L, 1, LUA_TYPE_PRNG);
  lua_pushinteger(L, prng_nexti(p));
  return 1;
}

static int L_PrngMt_nextf(lua_State* L)
{
  Prng* p = (Prng*)read_ldata(L, 1, LUA_TYPE_PRNG);
  lua_pushnumber(L, prng_nextf(p));
  return 1;
}

static int L_PrngMt_rangei(lua_State* L)
{
  int arg_count = lua_gettop(L) - 1;
  
  int lower;
  int upper;

  if (arg_count == 1) {
    lower = 0;
    upper = luaL_checkinteger(L, 2);
  } else {
    lower = luaL_checkinteger(L, 2);
    upper = luaL_checkinteger(L, 3);
  }

  Prng* p = (Prng*)read_ldata(L, 1, LUA_TYPE_PRNG);
  lua_pushinteger(L, prng_get_rangei(p, lower, upper));
  return 1;
}

static int L_PrngMt_rangef(lua_State* L)
{
  int arg_count = lua_gettop(L) - 1;
  
  double lower;
  double upper;

  if (arg_count == 1) {
    lower = 0;
    upper = luaL_checknumber(L, 2);
  } else {
    lower = luaL_checknumber(L, 2);
    upper = luaL_checknumber(L, 3);
  }

  Prng* p = (Prng*)read_ldata(L, 1, LUA_TYPE_PRNG);
  lua_pushinteger(L, prng_get_rangef(p, lower, upper));
  return 1;
}

static int L_PrngMt__gc(lua_State* L)
{
  Prng* p = (Prng*)read_ldata(L, 1, LUA_TYPE_PRNG);
  mem_destroy(p);
  return 0;
}

luaL_Reg prng_mt[] = {
  {"nexti", L_PrngMt_nexti},
  {"nextf", L_PrngMt_nextf},
  {"rangei", L_PrngMt_rangei},
  {"rangef", L_PrngMt_rangef},
  {"__index", L_PrngMt__index},
  {"__gc", L_PrngMt__gc},
  {NULL, NULL},
};

void wrap_prng(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  reg_funcs(L, prng_funcs);

  luaL_newmetatable(L, PRNG_MT_NAME);
  reg_funcs(L, prng_mt);

  lua_pop(L, 2);
}
