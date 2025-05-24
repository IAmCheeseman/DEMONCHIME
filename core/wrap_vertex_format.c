#include "wrap_vertex_format.h"

#include "core_mem.h"

static int L_create_vert_fmt(lua_State* L)
{
  lvertex_format_t* lua_fmt =
    (lvertex_format_t*)mem_alloc(sizeof(lvertex_format_t));
  lua_fmt->attribs = NULL;
  lua_fmt->count = 0;
  lua_fmt->capacity = 0;
  lua_fmt->fmt = vert_fmt_create(lua_fmt->attribs, lua_fmt->count);
  create_ldata(L, lua_fmt, vert_fmt_mt_name, lua_type_vert_fmt);
  return 1;
}

luaL_Reg vertex_format_funcs[] = {
  {"create_vert_fmt", L_create_vert_fmt},
  {NULL, NULL},
};

static int L_VertexFormatMt_add_attr(lua_State* L)
{
  lvertex_format_t* lua_fmt = 
    (lvertex_format_t*)read_ldata(L, 1, lua_type_vert_fmt);

  size_t type_name_len;
  const char* type_name = luaL_checklstring(L, 2, &type_name_len);

  data_type_t type = str_to_data_type(type_name, type_name_len);
  int components = luaL_checkinteger(L, 3);

  if (lua_fmt->count + 1 > lua_fmt->capacity) {
    lua_fmt->capacity = grow_capacity(lua_fmt->capacity);
    lua_fmt->attribs = (vert_attr_t*)mem_realloc(
      lua_fmt->attribs,
      sizeof(vert_attr_t) * lua_fmt->capacity);
  }

  vert_attr_t attrib;
  attrib.type = type;
  attrib.components = components;
  lua_fmt->attribs[lua_fmt->count++] = attrib;

  vert_fmt_update(&lua_fmt->fmt, lua_fmt->attribs, lua_fmt->count);

  return 0;
}

static int L_VertexFormatMt__index(lua_State* L)
{
  luaL_getmetatable(L, vert_fmt_mt_name);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_VertexFormatMt__gc(lua_State* L)
{
  lvertex_format_t* fmt =
    (lvertex_format_t*)read_ldata(L, 1, lua_type_vert_fmt);
  mem_destroy(fmt->attribs);
  mem_destroy(fmt);
  return 0;
}

luaL_Reg vertex_format_mt[] = {
  {"add_attr", L_VertexFormatMt_add_attr},
  {"__index", L_VertexFormatMt__index},
  {"__gc", L_VertexFormatMt__gc},
  {NULL, NULL},
};

void wrap_vert_fmt(lua_State* L)
{
  lua_getglobal(L, core_name);
  reg_funcs(L, vertex_format_funcs);

  luaL_newmetatable(L, vert_fmt_mt_name);
  reg_funcs(L, vertex_format_mt);

  lua_pop(L, 2);
}
