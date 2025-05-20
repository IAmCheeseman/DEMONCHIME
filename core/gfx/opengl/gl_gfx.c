#include "gfx/gfx.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gfx/backend.h"
#include "gl_buffer_object.h"
#include "gl_vertex_array.h"
#include "gl_shader.h"
#include "gl_framebuffer.h"
#include "gl_texture.h"
#include "mem.h"

static void MessageCallback(
  GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam)
{
  switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      // LogDebug("[" TEXT_DARK_GRAY "OpenGL" TEXT_NORMAL "] %s", message);
      return;
    default:
      LogWarning("[" TEXT_DARK_GRAY "OpenGL" TEXT_NORMAL "] %s", message);
      return;
  }
}

static void gl_ClearBackground(float r, float g, float b)
{
  glClearColor(r, g, b, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void gl_AdjustViewport(vec2f_t size)
{
  glViewport(0, 0, size.x, size.y);
}

static void gl_SetDepthTest(bool test)
{
  if (test) glEnable(GL_DEPTH_TEST);
  else glDisable(GL_DEPTH_TEST);
}

static void SetBackendPtrs(struct Renderer* r)
{
  r->backend.clear_background = gl_ClearBackground;
  r->backend.adjust_viewport = gl_AdjustViewport;
  r->backend.set_depth_test = gl_SetDepthTest;
  r->backend.buffer_object_create = gl_BufferObjectCreate;
  r->backend.buffer_object_destroy = gl_BufferObjectDestroy;
  r->backend.buffer_object_bind = gl_BufferObjectBind;
  r->backend.buffer_object_unbind = gl_BufferObjectUnbind;
  r->backend.buffer_object_set = gl_BufferObjectSet;
  r->backend.vertex_array_create = gl_VertexArrayCreate;
  r->backend.vertex_array_destroy = gl_VertexArrayDestroy;
  r->backend.vertex_array_bind = gl_VertexArrayBind;
  r->backend.vertex_array_draw = gl_VertexArrayDraw;
  r->backend.vertex_array_draw_indexed = gl_VertexArrayDrawIndexed;
  r->backend.framebuffer_create = gl_FramebufferCreate;
  r->backend.framebuffer_destroy = gl_FramebufferDestroy;
  r->backend.framebuffer_bind = gl_FramebufferBind;
  r->backend.framebuffer_resize = gl_FramebufferResize;
  r->backend.framebuffer_draw = gl_FramebufferDraw;
  r->backend.shader_load_files = gl_ShaderLoadFromFiles;
  r->backend.shader_load_src = gl_ShaderLoadFromSource;
  r->backend.shader_destroy = gl_ShaderDestroy;
  r->backend.shader_send_int = gl_ShaderSendInt;
  r->backend.shader_send_float = gl_ShaderSendFloat;
  r->backend.shader_send_vec2f = gl_ShaderSendVec2f;
  r->backend.shader_send_vec2i = gl_ShaderSendVec2i;
  r->backend.shader_send_vec3f = gl_ShaderSendVec3f;
  r->backend.shader_send_vec3i = gl_ShaderSendVec3i;
  r->backend.shader_send_vec4f = gl_ShaderSendVec4f;
  r->backend.shader_send_vec4i = gl_ShaderSendVec4i;
  r->backend.shader_send_mat4 = gl_ShaderSendMat4;
  r->backend.shader_bind = gl_ShaderBind;
  r->backend.texture_load_img = gl_TextureLoadFromImg;
  r->backend.texture_destroy = gl_TextureDestroy;
  r->backend.texture_gen_mipmaps = gl_TextureGenerateMipmaps;
  r->backend.texture_bind = gl_TextureBind;
  r->backend.texture_set_filter = gl_TextureSetFilter;
  r->backend.texture_set_wrap = gl_TextureSetWrap;
}

void gl_InitBackend(struct Engine* engine)
{
  glfwMakeContextCurrent(engine->window_handle);

  struct Renderer* renderer = (struct Renderer*)Alloc(sizeof(struct Renderer));
  SetBackendPtrs(renderer);

  engine->renderer = renderer;

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LogFatal(1, "could not initialize glad");
  }
  LogInfo("initialized glad");

#ifdef bse_debug
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
#endif

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // glFrontFace(GL_CW);
}

