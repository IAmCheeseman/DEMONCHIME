#include "gl_framebuffer.h"

#include <glad/glad.h>

#include "gfx/gfx_types.h"
#include "gfx/vertex_array.h"
#include "gl_shader.h"
#include "gl_vertex_array.h"
#include "gl_buffer_object.h"
#include "mem.h"

struct Shader* framebuffer_draw_shader = NULL;

static void GenerateTexture(
  struct Framebuffer* fb,
  uint32_t* handle, 
  GLenum attachment,
  GLenum internal,
  GLenum format,
  GLenum type)
{
  glGenTextures(1, handle);
  glBindTexture(GL_TEXTURE_2D, *handle);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    internal,
    fb->size.x,
    fb->size.y,
    0,
    format,
    type,
    NULL
  );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(
    GL_FRAMEBUFFER,
    attachment,
    GL_TEXTURE_2D,
    *handle,
    0
  );

  glBindTexture(GL_TEXTURE_2D, 0);
}

static void GenerateTextures(struct Framebuffer* fb)
{
  if (fb->flags & FRAMEBUFFER_COLOR_BUF) {
    GenerateTexture(
      fb,
      &fb->color_handle,
      GL_COLOR_ATTACHMENT0,
      GL_RGB,
      GL_RGB,
      GL_UNSIGNED_BYTE
    );
  }

  // TODO: maybe make depth/stencil use renderbuffers? they're faster when not 
  // being read and i doubt i'll be doing much reading from these
  if (fb->flags & FRAMEBUFFER_DEPTH_MASK_BUF) {
    GenerateTexture(
      fb,
      &fb->z_mask_handle,
      GL_DEPTH_STENCIL_ATTACHMENT,
      GL_DEPTH24_STENCIL8,
      GL_DEPTH_STENCIL,
      GL_UNSIGNED_INT_24_8
    );
  } else if (fb->flags & FRAMEBUFFER_DEPTH_BUF) {
    GenerateTexture(
      fb,
      &fb->z_mask_handle,
      GL_DEPTH_ATTACHMENT,
      GL_DEPTH_COMPONENT32F,
      GL_DEPTH_COMPONENT,
      GL_FLOAT
    );
  }
}

struct Framebuffer* gl_FramebufferCreate(
  struct Vfs* vfs,
  vec2i_t size,
  uint8_t flags)
{
  struct Framebuffer* fb =
    (struct Framebuffer*)Alloc(sizeof(struct Framebuffer));

  fb->size = size;
  fb->flags = flags;
  fb->color_handle = 0;
  fb->z_mask_handle = 0;
  fb->vao = NULL;
  fb->vbo = NULL;

  glGenFramebuffers(1, &fb->fb_handle);

  glBindFramebuffer(GL_FRAMEBUFFER, fb->fb_handle);

  GenerateTextures(fb);

  if (flags & FRAMEBUFFER_DRAWABLE) {
    struct VertexAttrib attribs[] = {
      {TYPE_FLOAT, 2}, // position
      {TYPE_FLOAT, 2}, // UV
    };
    struct VertexFormat format = VertexFormatCreate(attribs, 2);
    fb->vbo = gl_BufferObjectCreate(BUFFER_ARRAY);
    gl_BufferObjectSet(fb->vbo, NULL, 0, DRAW_DYNAMIC);
    gl_BufferObjectBind(fb->vbo);
    fb->vao = gl_VertexArrayCreate(&format);

    if (framebuffer_draw_shader == NULL) {
      framebuffer_draw_shader = gl_ShaderLoadFromFiles(
        vfs,
        "res/vfbdraw.glsl",
        "res/ffbdraw.glsl"
      );
    }
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    LogWarning("framebuffer %d could not be completed", fb->fb_handle);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  LogDebug(
    "created framebuffer %d (with color tex %d, depth/mask tex %d)",
    fb->fb_handle, fb->color_handle, fb->z_mask_handle);

  return fb;
}

void gl_FramebufferDestroy(struct Framebuffer* fb)
{
  if (fb->color_handle != 0) glDeleteTextures(1, &fb->color_handle);
  if (fb->z_mask_handle != 0) glDeleteTextures(1, &fb->z_mask_handle);
  if (fb->vao != NULL) gl_VertexArrayDestroy(fb->vao);
  if (fb->vbo != NULL) gl_BufferObjectDestroy(fb->vbo);
  glDeleteFramebuffers(1, &fb->fb_handle);
  Destroy(fb);
}

void gl_FramebufferBind(struct Framebuffer* fb)
{
  uint32_t handle = 0;
  if (fb != NULL) handle = fb->fb_handle;
  glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void gl_FramebufferResize(struct Framebuffer* fb, vec2i_t size)
{
  if (fb->color_handle != 0) glDeleteTextures(1, &fb->color_handle);
  if (fb->z_mask_handle != 0) glDeleteTextures(1, &fb->z_mask_handle);

  fb->size = size;

  glBindFramebuffer(GL_FRAMEBUFFER, fb->fb_handle);
  GenerateTextures(fb);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    LogWarning(
      "framebuffer %d resizing could not be completed",
      fb->fb_handle
    );
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gl_FramebufferDraw(struct Framebuffer* fb, vec2i_t start, vec2i_t end)
{
  if (fb->vbo == NULL || fb->vao == NULL) {
    LogWarning(
      "cannot draw framebuffer %d, because it is not drawable",
      fb->fb_handle
    );
    return;
  }

  float vertices[] = {
    start.x, start.y, 0, 0, // tl
    start.x + end.x, start.y, 1, 0, // tr
    start.x, start.y + end.y, 0, 1, // bl
    
    start.x + end.x, start.y, 1, 0, // tr
    start.x + end.x, start.y + end.y, 1, 1, // br
    start.x, start.y + end.y, 0, 1, // bl
  };

  gl_VertexArrayBind(fb->vao);
  gl_BufferObjectSet(fb->vbo, vertices, sizeof(vertices), DRAW_DYNAMIC);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fb->color_handle);

  gl_ShaderBind(framebuffer_draw_shader);
  gl_ShaderSendInt(framebuffer_draw_shader, "tex0", 0);
  gl_VertexArrayDraw(fb->vao, 0, 6, INDEX_TRIANGLES);
}
