#include "w_wrap.h"

#include "g_mesh.h"
#include "c_mem.h"

static int L_create_mesh(lua_State* L)
{
  int shader = luaL_checkinteger(L, 1);
  bool transparent = lua_toboolean(L, 2);

  mesh_t* mesh = (mesh_t*)mem_alloc(sizeof(mesh_t));
  *mesh = mesh_create(shader, transparent);
  create_ldata(L, mesh, mesh_mt_name, lua_type_mesh);
  return 1;
}

luaL_Reg mesh_funcs[] = {
  {"create_mesh", L_create_mesh},
  {NULL, NULL},
};

static int L_MeshMt_set_vertices(lua_State* L)
{
  mesh_t* mesh = (mesh_t*)read_ldata(L, 1, lua_type_mesh);
  luaL_checktype(L, 2, LUA_TTABLE);

  engine_t* engine = get_engine(L);

  const vert_fmt_t* fmt = &engine->renderer->shader_fmts[mesh->shader];

  size_t vertex_count = lua_objlen(L, 2);

  if (mesh->vertices != NULL) mem_destroy(mesh->vertices);

  mesh->vertices = mem_alloc(fmt->stride * vertex_count);
  mesh->vertex_count = vertex_count;

  // TODO: flatten this
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
          case type_unknown: luaL_error(L, "unknown type"); break;
          case type_half: luaL_error(L, "type 'half' not supported"); break;
          case type_float: AddAttrib(float, luaL_checknumber); break; 
          case type_double: AddAttrib(double, luaL_checknumber); break;
          case type_uchar: AddAttrib(unsigned char, luaL_checkinteger); break;
          case type_char: AddAttrib(signed char, luaL_checkinteger); break;
          case type_ushort:
            AddAttrib(unsigned short, luaL_checkinteger);
            break;
          case type_short: AddAttrib(short, luaL_checkinteger); break;
          case type_uint: AddAttrib(unsigned int, luaL_checkinteger); break;
          case type_int: AddAttrib(int, luaL_checkinteger); break;
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
  mesh_t* mesh = (mesh_t*)read_ldata(L, 1, lua_type_mesh);
  mesh_finalize(r, mesh, lua_toboolean(L, 2));
  return 0;
}

static int L_MeshMt_draw(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  mesh_t* mesh = (mesh_t*)read_ldata(L, 1, lua_type_mesh);
  tex_t* tex = (tex_t*)read_ldata(L, 2, lua_type_tex);
  lmat4_t* transform = (lmat4_t*)read_ldata(L, 3, lua_type_mat4);

  mesh_draw(r, mesh, tex, transform->m);
  return 0;
}

static int L_MeshMt__index(lua_State* L)
{
  luaL_getmetatable(L, mesh_mt_name);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_MeshMt__gc(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  mesh_t* mesh = (mesh_t*)read_ldata(L, 1, lua_type_mesh);
  if (mesh->vertex_count != 0) {
    mem_destroy(mesh->vertices);
    mesh->vertex_count = 0;
  }
  // if (mesh->index_count != 0) {
  //   mem_destroy(mesh->indices);
  //   mesh->index_count = 0;
  // }
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
  lua_getglobal(L, core_name);
  reg_funcs(L, mesh_funcs);

  luaL_newmetatable(L, mesh_mt_name);
  reg_funcs(L, mesh_mt);
  lua_pushstring(L, "mesh");
  lua_setfield(L, -2, "__type");

  lua_pop(L, 2);
}
