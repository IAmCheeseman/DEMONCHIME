#include "wrap_vertex_format.h"

#include "mem.h"

static int L_CreateVertexFormat(lua_State* L)
{
  struct LuaVertexFormat* lua_fmt =
    (struct LuaVertexFormat*)Alloc(sizeof(struct LuaVertexFormat));
  lua_fmt->attribs = NULL;
  lua_fmt->count = 0;
  lua_fmt->capacity = 0;
  lua_fmt->fmt = VertexFormatCreate(lua_fmt->attribs, lua_fmt->count);
  CreateLuaData(L, lua_fmt, VERTEX_FORMAT_MT_NAME, LUA_TYPE_VERTEX_FORMAT);
  return 1;
}

luaL_Reg vertex_format_funcs[] = {
  {"CreateVertexFormat", L_CreateVertexFormat},
  {NULL, NULL},
};

static int L_VertexFormatMt_AddAttrib(lua_State* L)
{
  struct LuaVertexFormat* lua_fmt = 
    (struct LuaVertexFormat*)ReadLuaData(L, 1, LUA_TYPE_VERTEX_FORMAT);

  size_t type_name_len;
  const char* type_name = luaL_checklstring(L, 2, &type_name_len);

  enum GfxDataType type = StringToDataType(type_name, type_name_len);
  int components = luaL_checkinteger(L, 3);

  if (lua_fmt->count + 1 > lua_fmt->capacity) {
    lua_fmt->capacity = GrowCapacity(lua_fmt->capacity);
    lua_fmt->attribs = (struct VertexAttrib*)Realloc(
      lua_fmt->attribs,
      sizeof(struct VertexAttrib) * lua_fmt->capacity);
  }

  struct VertexAttrib attrib;
  attrib.type = type;
  attrib.components = components;
  lua_fmt->attribs[lua_fmt->count++] = attrib;

  VertexFormatUpdate(&lua_fmt->fmt, lua_fmt->attribs, lua_fmt->count);

  return 0;
}

static int L_VertexFormatMt__index(lua_State* L)
{
  luaL_getmetatable(L, VERTEX_FORMAT_MT_NAME);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_VertexFormatMt__gc(lua_State* L)
{
  struct LuaVertexFormat* fmt =
    (struct LuaVertexFormat*)ReadLuaData(L, 1, LUA_TYPE_VERTEX_FORMAT);
  Destroy(fmt->attribs);
  Destroy(fmt);
  return 0;
}

luaL_Reg vertex_format_mt[] = {
  {"AddAttrib", L_VertexFormatMt_AddAttrib},
  {"__index", L_VertexFormatMt__index},
  {"__gc", L_VertexFormatMt__gc},
  {NULL, NULL},
};

void WrapVertexFormat(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  RegisterFunctions(L, vertex_format_funcs);

  luaL_newmetatable(L, VERTEX_FORMAT_MT_NAME);
  RegisterFunctions(L, vertex_format_mt);

  lua_pop(L, 2);
}
