#include "wrap.h"

#include "prng.h"
#include "mem.h"

static int L_CreatePrng(lua_State* L)
{
  Prng* p = (Prng*)Alloc(sizeof(Prng));
  *p = PrngCreate(luaL_optinteger(L, 1, 0));
  CreateLuaData(L, p, PRNG_MT_NAME, LUA_TYPE_PRNG);
  return 1;
}

luaL_Reg prng_funcs[] = {
  {"CreatePrng", L_CreatePrng},
  {NULL, NULL},
};

static int L_PrngMt__index(lua_State* L)
{
  luaL_getmetatable(L, PRNG_MT_NAME);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_PrngMt_NextI(lua_State* L)
{
  Prng* p = (Prng*)ReadLuaData(L, 1, LUA_TYPE_PRNG);
  lua_pushinteger(L, PrngGetInt(p));
  return 1;
}

static int L_PrngMt_NextF(lua_State* L)
{
  Prng* p = (Prng*)ReadLuaData(L, 1, LUA_TYPE_PRNG);
  lua_pushinteger(L, PrngGetDouble(p));
  return 1;
}

static int L_PrngMt_RangeI(lua_State* L)
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

  Prng* p = (Prng*)ReadLuaData(L, 1, LUA_TYPE_PRNG);
  lua_pushinteger(L, PrngGetIntRange(p, lower, upper));
  return 1;
}

static int L_PrngMt_RangeF(lua_State* L)
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

  Prng* p = (Prng*)ReadLuaData(L, 1, LUA_TYPE_PRNG);
  lua_pushinteger(L, PrngGetDoubleRange(p, lower, upper));
  return 1;
}

static int L_PrngMt__gc(lua_State* L)
{
  Prng* p = (Prng*)ReadLuaData(L, 1, LUA_TYPE_PRNG);
  Destroy(p);
  return 0;
}

luaL_Reg prng_mt[] = {
  {"NextI", L_PrngMt_NextI},
  {"NextF", L_PrngMt_NextF},
  {"RangeI", L_PrngMt_RangeI},
  {"RangeF", L_PrngMt_RangeF},
  {"__index", L_PrngMt__index},
  {"__gc", L_PrngMt__gc},
  {NULL, NULL},
};

void WrapPrng(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  RegisterFunctions(L, prng_funcs);

  luaL_newmetatable(L, PRNG_MT_NAME);
  RegisterFunctions(L, prng_mt);

  lua_pop(L, 2);
}
