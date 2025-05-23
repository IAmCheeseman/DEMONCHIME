#include "gfx/gfx.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gfx/backend.h"
#include "gl_buffer_object.h"
#include "gl_vertex_array.h"
#include "gl_shader.h"
#include "gl_framebuffer.h"
#include "gl_texture.h"
#include "gl_text.h"
#include "mem.h"

static void msg_callback(
  GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* msg,
  const void* user_param)
{
  unused(source);
  unused(type);
  unused(id);
  unused(length);
  unused(user_param);

  switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      // log_debug("[" TEXT_DARK_GRAY "OpenGL" TEXT_NORMAL "] %s", message);
      return;
    default:
      log_warning("[" TEXT_DARK_GRAY "OpenGL" TEXT_NORMAL "] %s", msg);
      return;
  }
}

static void gl_clear_bg(float r, float g, float b)
{
  glClearColor(r, g, b, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void gl_adjust_viewport(vec2f_t size)
{
  glViewport(0, 0, size.x, size.y);
}

static void gl_set_depth_test(bool test)
{
  if (test) glEnable(GL_DEPTH_TEST);
  else glDisable(GL_DEPTH_TEST);
}

static void set_backend_ptrs(renderer_t* r)
{
  r->backend.clear_background = gl_clear_bg;
  r->backend.adjust_viewport = gl_adjust_viewport;
  r->backend.set_depth_test = gl_set_depth_test;
  r->backend.buf_obj_create = gl_buf_obj_create;
  r->backend.buf_obj_destroy = gl_buf_obj_destroy;
  r->backend.buf_obj_bind = gl_buf_obj_bind;
  r->backend.buf_obj_unbind = gl_buf_obj_unbind;
  r->backend.buf_obj_set = gl_buf_obj_set_dat;
  r->backend.vert_arr_create = gl_vert_arr_create;
  r->backend.vert_arr_destroy = gl_vert_arr_destroy;
  r->backend.vert_arr_bind = gl_vert_arr_bind;
  r->backend.vert_arr_draw = gl_vert_arr_draw;
  r->backend.vert_arr_draw_idx = gl_vert_arr_draw_idx;
  r->backend.framebuf_create = gl_framebuf_create;
  r->backend.framebuf_destroy = gl_framebuf_destroy;
  r->backend.framebuf_bind = gl_framebuf_bind;
  r->backend.framebuf_resize = gl_framebuf_resize;
  r->backend.framebuf_draw = gl_framebuf_draw;
  r->backend.shader_load_files = gl_shader_load_from_files;
  r->backend.shader_load_src = gl_shader_load_from_src;
  r->backend.shader_destroy = gl_shader_destroy;
  r->backend.shader_send_int = gl_shader_send_int;
  r->backend.shader_send_float = gl_shader_send_float;
  r->backend.shader_send_vec2f = gl_shader_send_vec2f;
  r->backend.shader_send_vec2i = gl_shader_send_vec2i;
  r->backend.shader_send_vec3f = gl_shader_send_vec3f;
  r->backend.shader_send_vec3i = gl_shader_send_vec3i;
  r->backend.shader_send_vec4f = gl_shader_send_vec4f;
  r->backend.shader_send_vec4i = gl_shader_send_vec4i;
  r->backend.shader_send_mat4 = gl_shader_send_mat4;
  r->backend.shader_bind = gl_shader_bind;
  r->backend.texture_load_img = gl_tex_load_from_img;
  r->backend.texture_destroy = gl_tex_destroy;
  r->backend.texture_gen_mipmaps = gl_tex_gen_mipmap;
  r->backend.texture_bind = gl_tex_bind;
  r->backend.texture_set_filter = gl_tex_set_filter;
  r->backend.texture_set_wrap = gl_tex_set_wrap;
  r->backend.font_init = gl_font_init;
  r->backend.font_destroy = gl_font_destroy;
  r->backend.font_draw = gl_font_draw;
}

void gl_init_backend(engine_t* engine)
{
  glfwMakeContextCurrent(engine->window.handle);

  renderer_t* renderer = (renderer_t*)mem_alloc(sizeof(renderer_t));
  set_backend_ptrs(renderer);

  engine->renderer = renderer;

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    log_fatal(1, "could not initialize glad");
  }
  log_info("initialized glad");

#ifdef bse_debug
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(msg_callback, 0);
#endif

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // glFrontFace(GL_CW);
}

