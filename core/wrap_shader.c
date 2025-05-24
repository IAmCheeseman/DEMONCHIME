#include "wrap.h"

#include "gfx_shader.h"
#include "core_mem.h"

static int L_load_shader(lua_State* L)
{
  engine_t* engine = get_engine(L);
  const char* vert_path = luaL_checkstring(L, 1);
  const char* frag_path = luaL_checkstring(L, 2);
  shader_t* shader = shader_load_from_files(
    engine->renderer,
    engine->vfs,
    vert_path,
    frag_path
  );
  create_ldata(L, shader, shader_mt_name, lua_type_shader);
  return 1;
}

static int L_load_shader_from_src(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  const char* vert_src = luaL_checkstring(L, 1);
  const char* frag_src = luaL_checkstring(L, 2);
  shader_t* shader = shader_load_from_src(r, vert_src, frag_src);
  create_ldata(L, shader, shader_mt_name, lua_type_shader);
  return 1;
}

luaL_Reg shader_funcs[] = {
  {"load_shader", L_load_shader},
  {"load_shader_from_src", L_load_shader_from_src},
  {NULL, NULL},
};

static int L_ShaderMt_sendf(lua_State* L)
{
  int arg_count = lua_gettop(L) - 2;
  if (arg_count > 4) {
    luaL_error(L, "cannot send more than 4 floats to shader");
  }

  shader_t* shader = (shader_t*)read_ldata(L, 1, lua_type_shader);
  const char* uniform = luaL_checkstring(L, 2);

  float args[4];
  for (int i = 0; i < arg_count; i++)
    args[i] = luaL_checknumber(L, i + 3);

  renderer_t* r = get_engine(L)->renderer;
  switch (arg_count) {
    case 1: shader_send_float(r, shader, uniform, args[0]); break;
    case 2: shader_send_vec2f(r, shader, uniform, *((vec2f_t*)&args)); break;
    case 3: shader_send_vec3f(r, shader, uniform, *((vec3f_t*)&args)); break;
    case 4: shader_send_vec4f(r, shader, uniform, *((vec4f_t*)&args)); break;
  }

  return 0;
}

static int L_ShaderMt_sendi(lua_State* L)
{
  int arg_count = lua_gettop(L) - 2;
  if (arg_count > 4) {
    luaL_error(L, "cannot send more than 4 floats to shader");
  }

  shader_t* shader = (shader_t*)read_ldata(L, 1, lua_type_shader);
  const char* uniform = luaL_checkstring(L, 2);

  int args[4];
  for (int i = 0; i < arg_count; i++)
    args[i] = luaL_checkinteger(L, i + 3);

  renderer_t* r = get_engine(L)->renderer;
  switch (arg_count) {
    case 1: shader_send_int(r, shader, uniform, args[0]); break;
    case 2: shader_send_vec2i(r, shader, uniform, *((vec2i_t*)&args)); break;
    case 3: shader_send_vec3i(r, shader, uniform, *((vec3i_t*)&args)); break;
    case 4: shader_send_vec4i(r, shader, uniform, *((vec4i_t*)&args)); break;
  }

  return 0;
}

static int L_ShaderMt_send_mat4(lua_State* L)
{
  shader_t* shader = (shader_t*)read_ldata(L, 1, lua_type_shader);
  const char* uniform = luaL_checkstring(L, 2);
  lmat4_t* m = (lmat4_t*)read_ldata(L, 3, lua_type_mat4);
  
  renderer_t* r = get_engine(L)->renderer;
  shader_send_mat4(r, shader, uniform, m->m);

  return 0;
}

static int L_ShaderMt_bind(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  shader_t* shader = (shader_t*)read_ldata(L, 1, lua_type_shader);
  shader_bind(r, shader);
  return 0;
}

static int L_ShaderMt__index(lua_State* L)
{
  luaL_getmetatable(L, shader_mt_name);
  lua_getfield(L, -1, luaL_checkstring(L, 2));
  return 1;
}

static int L_ShaderMt__gc(lua_State* L)
{
  renderer_t* r = get_engine(L)->renderer;
  shader_t* shader = (shader_t*)read_ldata(L, 1, lua_type_shader);
  shader_destroy(r, shader);
  return 0;
}

luaL_Reg shader_mt[] = {
  {"sendf", L_ShaderMt_sendf},
  {"sendi", L_ShaderMt_sendi},
  {"send_mat4", L_ShaderMt_send_mat4},
  {"bind", L_ShaderMt_bind},
  {"__index", L_ShaderMt__index},
  {"__gc", L_ShaderMt__gc},
  {NULL, NULL},
};

void wrap_shader(lua_State* L)
{
  lua_getglobal(L, core_name);
  reg_funcs(L, shader_funcs);

  luaL_newmetatable(L, shader_mt_name);
  reg_funcs(L, shader_mt);

  lua_pop(L, 2);
}
