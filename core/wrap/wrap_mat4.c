#include "wrap.h"

#include "math/mat4.h"
#include "math/transform.h"
#include "math/vec3f.h"
#include "mem.h"

static int L_Mat4Identity(lua_State* L)
{
  struct LuaMat4* m = (struct LuaMat4*)Alloc(sizeof(struct LuaMat4));
  Mat4Identity(m->m);
  CreateLuaData(L, m, MAT4_MT_NAME, LUA_TYPE_MAT4);
  return 1;
}

static int L_Mat4FromTransform(lua_State* L)
{
  luaL_checktype(L, 1, LUA_TTABLE);

  lua_getfield(L, 1, "x");
  float x = luaL_optnumber(L, -1, 0);
  lua_pop(L, 1);
  lua_getfield(L, 1, "y");
  float y = luaL_optnumber(L, -1, 0);
  lua_pop(L, 1);
  lua_getfield(L, 1, "z");
  float z = luaL_optnumber(L, -1, 0);
  lua_pop(L, 1);

  lua_getfield(L, 1, "rx");
  float rx = luaL_optnumber(L, -1, 0);
  lua_pop(L, 1);
  lua_getfield(L, 1, "ry");
  float ry = luaL_optnumber(L, -1, 0);
  lua_pop(L, 1);
  lua_getfield(L, 1, "rz");
  float rz = luaL_optnumber(L, -1, 0);
  lua_pop(L, 1);

  lua_getfield(L, 1, "sx");
  float sx = luaL_optnumber(L, -1, 1);
  lua_pop(L, 1);
  lua_getfield(L, 1, "sy");
  float sy = luaL_optnumber(L, -1, 1);
  lua_pop(L, 1);
  lua_getfield(L, 1, "sz");
  float sz = luaL_optnumber(L, -1, 1);
  lua_pop(L, 1);

  struct Transform trans;
  trans.pos = (vec3f_t){x, y, z};
  trans.rot = (vec3f_t){rx, ry, rz};
  trans.scale = (vec3f_t){sx, sy, sz};

  struct LuaMat4* m = (struct LuaMat4*)Alloc(sizeof(struct LuaMat4));
  TransformToMatrix(trans, m->m);
  CreateLuaData(L, m, MAT4_MT_NAME, LUA_TYPE_MAT4);
  return 1;
}

luaL_Reg mat4_funcs[] = {
  {"Mat4Identity", L_Mat4Identity},
  {"Mat4FromTransform", L_Mat4FromTransform},
  {NULL, NULL},
};

static int L_Mat4Mt_Multiply(lua_State* L)
{
  struct LuaMat4* m0 = (struct LuaMat4*)ReadLuaData(L, 1, LUA_TYPE_MAT4);
  struct LuaMat4* m1 = (struct LuaMat4*)ReadLuaData(L, 2, LUA_TYPE_MAT4);
  
  struct LuaMat4* mout = (struct LuaMat4*)Alloc(sizeof(struct LuaMat4));
  Mat4Multiply(mout->m, m0->m, m1->m);
  CreateLuaData(L, mout, MAT4_MT_NAME, LUA_TYPE_MAT4);
  return 1;
}

static int L_Mat4Mt_Translate(lua_State* L)
{
  struct LuaMat4* m = (struct LuaMat4*)ReadLuaData(L, 1, LUA_TYPE_MAT4);
  
  float x = luaL_checknumber(L, 2);
  float y = luaL_checknumber(L, 3);
  float z = luaL_checknumber(L, 4);

  Mat4Translate(m->m, (vec3f_t){x, y, z});

  return 0;
}

static int L_Mat4Mt_Scale(lua_State* L)
{
  struct LuaMat4* m = (struct LuaMat4*)ReadLuaData(L, 1, LUA_TYPE_MAT4);
  
  float x = luaL_checknumber(L, 2);
  float y = luaL_checknumber(L, 3);
  float z = luaL_checknumber(L, 4);

  Mat4Scale(m->m, (vec3f_t){x, y, z});

  return 0;
}

static int L_Mat4Mt_Rotate(lua_State* L)
{
  struct LuaMat4* m = (struct LuaMat4*)ReadLuaData(L, 1, LUA_TYPE_MAT4);
  
  float x = luaL_checknumber(L, 2);
  float y = luaL_checknumber(L, 3);
  float z = luaL_checknumber(L, 4);

  Mat4Rotate(m->m, (vec3f_t){x, y, z});

  return 0;
}

static int L_Mat4Mt_Perspective(lua_State* L)
{
  struct LuaMat4* m = (struct LuaMat4*)ReadLuaData(L, 1, LUA_TYPE_MAT4);
  
  float yfov = luaL_checknumber(L, 2);
  float a = luaL_checknumber(L, 3);
  float n = luaL_checknumber(L, 4);
  float f = luaL_checknumber(L, 5);

  Mat4Perspective(m->m, yfov, a, n, f);

  return 0;
}

static int L_Mat4Mt_Ortho(lua_State* L)
{
  struct LuaMat4* m = (struct LuaMat4*)ReadLuaData(L, 1, LUA_TYPE_MAT4);
  
  float l = luaL_checknumber(L, 3);
  float r = luaL_checknumber(L, 4);
  float b = luaL_checknumber(L, 5);
  float t = luaL_checknumber(L, 6);
  float n = luaL_checknumber(L, 7);
  float f = luaL_checknumber(L, 8);

  Mat4Ortho(m->m, l, r, b, t, n, f);

  return 0;
}

static int L_Mat4Mt__index(lua_State* L)
{
  luaL_getmetatable(L, MAT4_MT_NAME);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_Mat4Mt__gc(lua_State* L)
{
  struct LuaMat4* m = (struct LuaMat4*)ReadLuaData(L, 1, LUA_TYPE_MAT4);
  Destroy(m);
  return 0;
}

luaL_Reg mat4_mt[] = {
  {"Multiply", L_Mat4Mt_Multiply},
  {"Translate", L_Mat4Mt_Translate},
  {"Scale", L_Mat4Mt_Scale},
  {"Rotate", L_Mat4Mt_Rotate},
  {"Perspective", L_Mat4Mt_Perspective},
  {"Ortho", L_Mat4Mt_Ortho},
  {"__index", L_Mat4Mt__index},
  {"__gc", L_Mat4Mt__gc},
  {NULL, NULL},
};

void WrapMat4(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  RegisterFunctions(L, mat4_funcs);

  luaL_newmetatable(L, MAT4_MT_NAME);
  RegisterFunctions(L, mat4_mt);

  lua_pop(L, 2);
}
