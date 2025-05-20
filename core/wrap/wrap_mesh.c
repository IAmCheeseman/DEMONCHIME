#include "wrap.h"

#include "gfx/mesh.h"
#include "mem.h"
#include "wrap_vertex_format.h"

static int L_create_mesh(lua_State* L)
{
  lvertex_format_t* lua_fmt = 
    (lvertex_format_t*)read_ldata(L, 1, LUA_TYPE_VERT_FMT);

  mesh_t* mesh = (mesh_t*)mem_alloc(sizeof(mesh_t));
  *mesh = mesh_create(&lua_fmt->fmt);
  create_ldata(L, mesh, MESH_MT_NAME, LUA_TYPE_MESH);
  return 1;
}

luaL_Reg mesh_funcs[] = {
  {"create_mesh", L_create_mesh},
  {NULL, NULL},
};

static int L_MeshMt_set_vertices(lua_State* L)
{
  mesh_t* mesh = (mesh_t*)read_ldata(L, 1, LUA_TYPE_MESH);
  luaL_checktype(L, 2, LUA_TTABLE);

  vert_fmt_t* fmt = mesh->fmt;

  size_t vertex_count = lua_objlen(L, 2);

  if (mesh->vertices != NULL) mem_destroy(mesh->vertices);

  mesh->vertices = mem_alloc(fmt->stride * vertex_count);
  mesh->vertex_count = vertex_count;

  for (size_t vertex_i = 0; vertex_i < vertex_count; vertex_i++) {
    lua_pushinteger(L, vertex_i + 1);
    lua_gettable(L, 2); // index mesh table
    if (lua_type(L, -1) != LUA_TTABLE) {
      mem_destroy(mesh->vertices);
      luaL_error(L, "vertices must be tables");
    }

    size_t offset = vertex_i * fmt->stride;
    int index = 1;

    for (size_t attr_i = 0; attr_i < fmt->attrib_count; attr_i++) {
      const vert_attr_t* attrib = &fmt->attribs[attr_i];
      size_t type_size = get_data_type_size(attrib->type);

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
          case TYPE_CHAR: AddAttrib(signed char, luaL_checkinteger); break;
          case TYPE_USHORT:
            AddAttrib(unsigned short, luaL_checkinteger);
            break;
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

static int L_MeshMt_finalize(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  mesh_t* mesh = (mesh_t*)read_ldata(L, 1, LUA_TYPE_MESH);
  mesh_finalize(r, mesh, lua_toboolean(L, 2));
  return 0;
}

static int L_MeshMt_draw(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  mesh_t* mesh = (mesh_t*)read_ldata(L, 1, LUA_TYPE_MESH);
  mesh_draw(r, mesh);
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
  renderer_t* r = get_engine(L)->renderer;
  mesh_t* mesh = (mesh_t*)read_ldata(L, 1, LUA_TYPE_MESH);
  if (mesh->vertex_count != 0) {
    mem_destroy(mesh->vertices);
    mesh->vertex_count = 0;
  }
  if (mesh->index_count != 0) {
    mem_destroy(mesh->indices);
    mesh->index_count = 0;
  }
  mesh_destroy(r, mesh);
  mem_destroy(mesh);
  return 0;
}

luaL_Reg mesh_mt[] = {
  {"set_vertices", L_MeshMt_set_vertices},
  {"finalize", L_MeshMt_finalize},
  {"draw", L_MeshMt_draw},
  {"__index", L_MeshMt__index},
  {"__gc", L_MeshMt__gc},
  {NULL, NULL},
};

void wrap_mesh(lua_State* L)
{
  lua_getglobal(L, CORE_NAME);
  reg_funcs(L, mesh_funcs);

  luaL_newmetatable(L, MESH_MT_NAME);
  reg_funcs(L, mesh_mt);

  lua_pop(L, 2);
}
