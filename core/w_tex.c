#include "w_wrap.h"

#include "g_tex.h"
#include "c_mem.h"

static int L_load_tex(lua_State* L)
{
  engine_t* engine = get_engine(L);
  const char* path = luaL_checkstring(L, 1);
  tex_t* tex = (tex_t*)mem_alloc(sizeof(tex_t));
  *tex = tex_load(engine->renderer, engine->vfs, path);
  create_ldata(L, tex, tex_mt_name, lua_type_tex);
  return 1;
}

static int L_create_1x1_tex(lua_State* L)
{
  engine_t* engine = get_engine(L);

  color_t color;
  color.r = luaL_checknumber(L, 1);
  color.g = luaL_checknumber(L, 2);
  color.b = luaL_checknumber(L, 3);
  color.a = luaL_optnumber(L, 4, 1);

  tex_t* tex = (tex_t*)mem_alloc(sizeof(tex_t));
  *tex = tex_1x1_color(engine->renderer, color);
  create_ldata(L, tex, tex_mt_name, lua_type_tex);
  return 1;
}

luaL_Reg texture_funcs[] = {
  {"load_tex", L_load_tex},
  {"create_1x1_tex", L_create_1x1_tex},
  {NULL, NULL},
};

static int L_TextureMt_get_size(lua_State* L)
{
  tex_t* tex = (tex_t*)read_ldata(L, 1, lua_type_tex);
  lua_pushinteger(L, tex->size.x);
  lua_pushinteger(L, tex->size.y);
  return 2;
}

static int L_TextureMt_gen_mipmap(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  tex_t* tex = (tex_t*)read_ldata(L, 1, lua_type_tex);
  tex_gen_mipmap(r, tex);
  return 0;
}

static int L_TextureMt_set_filter(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  tex_t* tex = (tex_t*)read_ldata(L, 1, lua_type_tex);
  tex_filter_t min = luaL_checkinteger(L, 2);
  tex_filter_t mag = luaL_checkinteger(L, 3);
  tex_set_filter(r, tex, min, mag);
  return 0;
}

static int L_TextureMt_set_wrap(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  tex_t* tex = (tex_t*)read_ldata(L, 1, lua_type_tex);
  tex_wrap_t x = luaL_checkinteger(L, 2);
  tex_wrap_t y = luaL_checkinteger(L, 3);
  tex_set_wrap(r, tex, x, y);
  return 0;
}

static int L_TextureMt_bind(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  tex_t* tex = (tex_t*)read_ldata(L, 1, lua_type_tex);
  tex_bind(r, tex, luaL_checkinteger(L, 2));
  return 2;
}

static int L_TextureMt__index(lua_State* L)
{
  luaL_getmetatable(L, tex_mt_name);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_TextureMt__gc(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  tex_t* tex = (tex_t*)read_ldata(L, 1, lua_type_tex);
  tex_destroy(r, tex);
  mem_destroy(tex);
  return 0;
}

luaL_Reg texture_mt[] = {
  {"get_size", L_TextureMt_get_size},
  {"gen_mipmap", L_TextureMt_gen_mipmap},
  {"set_filter", L_TextureMt_set_filter},
  {"set_wrap", L_TextureMt_set_wrap},
  {"bind", L_TextureMt_bind},
  {"__index", L_TextureMt__index},
  {"__gc", L_TextureMt__gc},
  {NULL, NULL},
};

void wrap_tex(lua_State* L)
{
  lua_getglobal(L, core_name);
  reg_funcs(L, texture_funcs);

  luaL_newmetatable(L, tex_mt_name);
  reg_funcs(L, texture_mt);
  lua_pushstring(L, "tex");
  lua_setfield(L, -2, "__type");

  lua_pop(L, 2);
}
