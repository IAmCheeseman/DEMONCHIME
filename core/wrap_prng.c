#include "wrap.h"

#include "core_prng.h"
#include "core_mem.h"

static int L_create_prng(lua_State* L)
{
  prng_t* p = (prng_t*)mem_alloc(sizeof(prng_t));
  *p = prng_create(luaL_optinteger(L, 1, 0));
  create_ldata(L, p, prng_mt_name, lua_type_prng);
  return 1;
}

luaL_Reg prng_funcs[] = {
  {"create_prng", L_create_prng},
  {NULL, NULL},
};

static int L_PrngMt__index(lua_State* L)
{
  luaL_getmetatable(L, prng_mt_name);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_PrngMt_nexti(lua_State* L)
{
  prng_t* p = (prng_t*)read_ldata(L, 1, lua_type_prng);
  lua_pushinteger(L, prng_nexti(p));
  return 1;
}

static int L_PrngMt_nextf(lua_State* L)
{
  prng_t* p = (prng_t*)read_ldata(L, 1, lua_type_prng);
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

  prng_t* p = (prng_t*)read_ldata(L, 1, lua_type_prng);
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

  prng_t* p = (prng_t*)read_ldata(L, 1, lua_type_prng);
  lua_pushinteger(L, prng_get_rangef(p, lower, upper));
  return 1;
}

static int L_PrngMt__gc(lua_State* L)
{
  prng_t* p = (prng_t*)read_ldata(L, 1, lua_type_prng);
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
  lua_getglobal(L, core_name);
  reg_funcs(L, prng_funcs);

  luaL_newmetatable(L, prng_mt_name);
  reg_funcs(L, prng_mt);

  lua_pop(L, 2);
}
