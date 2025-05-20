#include "wrap.h"

#include "gfx/mesh.h"
#include "mem.h"
#include "wrap_vertex_format.h"

static int L_CreateMesh(lua_State* L)
{
  struct LuaVertexFormat* lua_fmt = 
    (struct LuaVertexFormat*)ReadLuaData(L, 1, LUA_TYPE_VERTEX_FORMAT);

  struct Mesh* mesh = (struct Mesh*)Alloc(sizeof(struct Mesh));
  *mesh = MeshCreate(&lua_fmt->fmt);
  CreateLuaData(L, mesh, MESH_MT_NAME, LUA_TYPE_MESH);
  return 1;
}

luaL_Reg mesh_funcs[] = {
  {"CreateMesh", L_CreateMesh},
  {NULL, NULL},
};

static int L_MeshMt_SetVertices(lua_State* L)
{
  struct Mesh* mesh = (struct Mesh*)ReadLuaData(L, 1, LUA_TYPE_MESH);
  luaL_checktype(L, 2, LUA_TTABLE);

  struct VertexFormat* fmt = mesh->fmt;

  size_t vertex_count = lua_objlen(L, 2);

  if (mesh->vertices != NULL) {
    Destroy(mesh->vertices);
  }

  mesh->vertices = Alloc(fmt->stride * vertex_count);
  mesh->vertex_count = vertex_count;

  for (size_t vertex_i = 0; vertex_i < vertex_count; vertex_i++) {
    lua_pushinteger(L, vertex_i + 1);
    lua_gettable(L, 2); // index mesh table
    if (lua_type(L, -1) != LUA_TTABLE) {
      Destroy(mesh->vertices);
      luaL_error(L, "vertices must be tables");
    }

    size_t offset = vertex_i * fmt->stride;
    int index = 1;

    for (size_t attr_i = 0; attr_i < fmt->attrib_count; attr_i++) {
      const struct VertexAttrib* attrib = &fmt->attribs[attr_i];
      size_t type_size = GetGfxDataTypeSize(attrib->type);

      for (uint8_t i = 0; i < attrib->components; i++) {
        lua_pushinteger(L, index); // vertex table pushed back to -2
        lua_gettable(L, -2); // index vertex table

#define AddAttrib(T, fn) \
  do { \
    T f = fn(L, -1); \
    T* a = mesh->vertices + offset; \
    *a = f; \
  } while (false)

        switch (attrib->type) {
          case TYPE_UNKNOWN: luaL_error(L, "unknown type"); break;
          case TYPE_HALF: luaL_error(L, "type 'half' not supported"); break;
          case TYPE_FLOAT: AddAttrib(float, luaL_checknumber); break; 
          case TYPE_DOUBLE: AddAttrib(double, luaL_checknumber); break;
          case TYPE_UCHAR: AddAttrib(unsigned char, luaL_checkinteger); break;
          case TYPE_CHAR: AddAttrib(char, luaL_checkinteger); break;
          case TYPE_USHORT: AddAttrib(unsigned short, luaL_checkinteger); break;
          case TYPE_SHORT: AddAttrib(short, luaL_checkinteger); break;
          case TYPE_UINT: AddAttrib(unsigned int, luaL_checkinteger); break;
          case TYPE_INT: AddAttrib(int, luaL_checkinteger); break;
        }

#undef AddAttrib

        lua_pop(L, 1);
        offset += type_size;
        index++;
      }
    }
  }

  return 0;
}

static int L_MeshMt_Finalize(lua_State* L)
{
  struct Renderer* r = GetEngine(L)->renderer;
  struct Mesh* mesh = (struct Mesh*)ReadLuaData(L, 1, LUA_TYPE_MESH);
  MeshFinalize(r, mesh, lua_toboolean(L, 2));
  return 0;
}

static int L_MeshMt_Draw(lua_State* L)
{
  struct Renderer* r = GetEngine(L)->renderer;
  struct Mesh* mesh = (struct Mesh*)ReadLuaData(L, 1, LUA_TYPE_MESH);
  MeshDraw(r, mesh);
  return 0;
}

static int L_MeshMt__index(lua_State* L)
{
  luaL_getmetatable(L, MESH_MT_NAME);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_MeshMt__gc(lua_State* L)
{
  struct Renderer* r = GetEngine(L)->renderer;
  struct Mesh* mesh = (struct Mesh*)ReadLuaData(L, 1, LUA_TYPE_MESH);
  if (mesh->vertex_count != 0) {
    Destroy(mesh->vertices);
    mesh->vertex_count = 0;
  }
  if (mesh->index_count != 0) {
    Destroy(mesh->indices);
    mesh->index_count = 0;
  }
  MeshDestroy(r, mesh);
  Destroy(mesh);
  return 0;
}

luaL_Reg mesh_mt[] = {
  {"SetVertices", L_MeshMt_SetVertices},
  {"Finalize", L_MeshMt_Finalize},
  {"Draw", L_MeshMt_Draw},
  {"__index", L_MeshMt__index},
  {"__gc", L_MeshMt__gc},
  {NULL, NULL},
};

void WrapMesh(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  RegisterFunctions(L, mesh_funcs);

  luaL_newmetatable(L, MESH_MT_NAME);
  RegisterFunctions(L, mesh_mt);

  lua_pop(L, 2);
}
