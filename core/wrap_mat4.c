#include "wrap.h"

#include "mathx.h"
#include "core_mem.h"

static int L_mat4_identity(lua_State* L)
{
  lmat4_t* m = (lmat4_t*)mem_alloc(sizeof(lmat4_t));
  mat4_identity(m->m);
  create_ldata(L, m, mat4_mt_name, lua_type_mat4);
  return 1;
}

static int L_mat4_from_trans(lua_State* L)
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

  transform_t trans;
  trans.pos = (vec3f_t){x, y, z};
  trans.rot = (vec3f_t){rx, ry, rz};
  trans.scale = (vec3f_t){sx, sy, sz};

  lmat4_t* m = (lmat4_t*)mem_alloc(sizeof(lmat4_t));
  transform_to_matrix(trans, m->m);
  create_ldata(L, m, mat4_mt_name, lua_type_mat4);
  return 1;
}

luaL_Reg mat4_funcs[] = {
  {"mat4_identity", L_mat4_identity},
  {"mat4_from_trans", L_mat4_from_trans},
  {NULL, NULL},
};

static int L_Mat4Mt_mult(lua_State* L)
{
  lmat4_t* m0 = (lmat4_t*)read_ldata(L, 1, lua_type_mat4);
  lmat4_t* m1 = (lmat4_t*)read_ldata(L, 2, lua_type_mat4);
  
  lmat4_t* mout = (lmat4_t*)mem_alloc(sizeof(lmat4_t));
  mat4_mult(mout->m, m0->m, m1->m);
  create_ldata(L, mout, mat4_mt_name, lua_type_mat4);
  return 1;
}

static int L_Mat4Mt_translate(lua_State* L)
{
  lmat4_t* m = (lmat4_t*)read_ldata(L, 1, lua_type_mat4);
  
  float x = luaL_checknumber(L, 2);
  float y = luaL_checknumber(L, 3);
  float z = luaL_checknumber(L, 4);

  mat4_translate(m->m, (vec3f_t){x, y, z});

  return 0;
}

static int L_Mat4Mt_scale(lua_State* L)
{
  lmat4_t* m = (lmat4_t*)read_ldata(L, 1, lua_type_mat4);
  
  float x = luaL_checknumber(L, 2);
  float y = luaL_checknumber(L, 3);
  float z = luaL_checknumber(L, 4);

  mat4_scale(m->m, (vec3f_t){x, y, z});

  return 0;
}

static int L_Mat4Mt_rotate(lua_State* L)
{
  lmat4_t* m = (lmat4_t*)read_ldata(L, 1, lua_type_mat4);
  
  float x = luaL_checknumber(L, 2);
  float y = luaL_checknumber(L, 3);
  float z = luaL_checknumber(L, 4);

  mat4_rotate(m->m, (vec3f_t){x, y, z});

  return 0;
}

static int L_Mat4Mt_perspective(lua_State* L)
{
  lmat4_t* m = (lmat4_t*)read_ldata(L, 1, lua_type_mat4);
  
  float yfov = luaL_checknumber(L, 2);
  float a = luaL_checknumber(L, 3);
  float n = luaL_checknumber(L, 4);
  float f = luaL_checknumber(L, 5);

  mat4_perspective(m->m, yfov, a, n, f);

  return 0;
}

static int L_Mat4Mt_ortho(lua_State* L)
{
  lmat4_t* m = (lmat4_t*)read_ldata(L, 1, lua_type_mat4);
  
  float l = luaL_checknumber(L, 3);
  float r = luaL_checknumber(L, 4);
  float b = luaL_checknumber(L, 5);
  float t = luaL_checknumber(L, 6);
  float n = luaL_checknumber(L, 7);
  float f = luaL_checknumber(L, 8);

  mat4_ortho(m->m, l, r, b, t, n, f);

  return 0;
}

static int L_Mat4Mt__index(lua_State* L)
{
  luaL_getmetatable(L, mat4_mt_name);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_Mat4Mt__gc(lua_State* L)
{
  lmat4_t* m = (lmat4_t*)read_ldata(L, 1, lua_type_mat4);
  mem_destroy(m);
  return 0;
}

luaL_Reg mat4_mt[] = {
  {"mult", L_Mat4Mt_mult},
  {"translate", L_Mat4Mt_translate},
  {"scale", L_Mat4Mt_scale},
  {"rotate", L_Mat4Mt_rotate},
  {"perspective", L_Mat4Mt_perspective},
  {"ortho", L_Mat4Mt_ortho},
  {"__index", L_Mat4Mt__index},
  {"__gc", L_Mat4Mt__gc},
  {NULL, NULL},
};

void wrap_mat4(lua_State* L)
{
  lua_getglobal(L, core_name);
  reg_funcs(L, mat4_funcs);

  luaL_newmetatable(L, mat4_mt_name);
  reg_funcs(L, mat4_mt);

  lua_pop(L, 2);
}
