#include "w_wrap.h"

#include "g_font.h"

static int L_load_font(lua_State* L)
{
  engine_t* engine = get_engine(L);

  const char* path = luaL_checkstring(L, 1);
  int size = luaL_optinteger(L, 2, 28);

  font_t* font = font_load(engine->renderer, engine->vfs, path, size);
  create_ldata(L, font, font_mt_name, lua_type_font);
  return 1;
}

luaL_Reg font_funcs[] = {
  {"load_font", L_load_font},
  {NULL, NULL},
};

static int L_FontMt_get_width(lua_State* L)
{
  font_t* font = (font_t*)read_ldata(L, 1, lua_type_font);

  const char* text = luaL_checkstring(L, 2);

  engine_t* engine = get_engine(L);
  lua_pushinteger(L, font_get_width(engine->renderer, font, text));
  return 1;
}

static int L_FontMt_get_height(lua_State* L)
{
  font_t* font = (font_t*)read_ldata(L, 1, lua_type_font);
  engine_t* engine = get_engine(L);
  lua_pushinteger(L, font_get_height(engine->renderer, font));
  return 1;
}

static int L_FontMt_draw(lua_State* L)
{
  font_t* font = (font_t*)read_ldata(L, 1, lua_type_font);

  vec2f_t pos;
  pos.x = luaL_checknumber(L, 2);
  pos.y = luaL_checknumber(L, 3);
  const char* text = luaL_checkstring(L, 4);
  color_t color;
  color.r = luaL_optnumber(L, 5, 1);
  color.g = luaL_optnumber(L, 6, 1);
  color.b = luaL_optnumber(L, 7, 1);
  color.a = luaL_optnumber(L, 8, 1);

  engine_t* engine = get_engine(L);
  font_draw_colored(engine->renderer, font, pos, color, text);
  return 0;
}

static int L_FontMt__index(lua_State* L)
{
  luaL_getmetatable(L, font_mt_name);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_FontMt__gc(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  font_t* font = (font_t*)read_ldata(L, 1, lua_type_font);
  font_destroy(r, font);
  return 0;
}

luaL_Reg font_mt[] = {
  {"get_width", L_FontMt_get_width},
  {"get_height", L_FontMt_get_height},
  {"draw", L_FontMt_draw},
  {"__index", L_FontMt__index},
  {"__gc", L_FontMt__gc},
  {NULL, NULL},
};

void wrap_font(lua_State* L)
{
  lua_getglobal(L, core_name);
  reg_funcs(L, font_funcs);

  luaL_newmetatable(L, font_mt_name);
  reg_funcs(L, font_mt);
  lua_pushstring(L, "font");
  lua_setfield(L, -2, "__type");

  lua_pop(L, 2);
}
