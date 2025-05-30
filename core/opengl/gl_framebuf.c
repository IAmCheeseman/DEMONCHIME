#include "gl_framebuf.h"

#include <glad/glad.h>

#include "g_types.h"
#include "g_vert_arr.h"
#include "c_mem.h"
#include "gl_shader.h"
#include "gl_vert_arr.h"
#include "gl_buf_obj.h"

static void gen_tex(
  framebuf_t* fb,
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

static void gen_texs(framebuf_t* fb)
{
  if (fb->flags & framebuffer_color_buf) {
    gen_tex(
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
  if (fb->flags & framebuffer_depth_mask_buf) {
    gen_tex(
      fb,
      &fb->z_mask_handle,
      GL_DEPTH_STENCIL_ATTACHMENT,
      GL_DEPTH24_STENCIL8,
      GL_DEPTH_STENCIL,
      GL_UNSIGNED_INT_24_8
    );
  } else if (fb->flags & framebuffer_depth_buf) {
    gen_tex(
      fb,
      &fb->z_mask_handle,
      GL_DEPTH_ATTACHMENT,
      GL_DEPTH_COMPONENT32F,
      GL_DEPTH_COMPONENT,
      GL_FLOAT
    );
  }
}

framebuf_t* gl_framebuf_create(
  const renderer_t* r,
  vfs_t* vfs,
  vec2i_t size,
  uint8_t flags)
{
  framebuf_t* fb =
    (framebuf_t*)mem_alloc(sizeof(framebuf_t));

  fb->size = size;
  fb->flags = flags;
  fb->color_handle = 0;
  fb->z_mask_handle = 0;
  fb->vao = NULL;
  fb->vbo = NULL;

  glGenFramebuffers(1, &fb->fb_handle);

  glBindFramebuffer(GL_FRAMEBUFFER, fb->fb_handle);

  gen_texs(fb);

  if (flags & framebuffer_drawable) {
    fb->vbo = gl_buf_obj_create(buf_arr);
    gl_buf_obj_set_dat(fb->vbo, NULL, 0, draw_dynamic);
    gl_buf_obj_bind(fb->vbo);
    fb->vao = gl_vert_arr_create(r, shader_2d);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    log_warning("framebuffer %d could not be completed", fb->fb_handle);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  log_debug(
    "created framebuffer %d (with color tex %d, depth/mask tex %d)",
    fb->fb_handle, fb->color_handle, fb->z_mask_handle);

  return fb;
}

void gl_framebuf_destroy(framebuf_t* fb)
{
  if (fb->color_handle != 0) glDeleteTextures(1, &fb->color_handle);
  if (fb->z_mask_handle != 0) glDeleteTextures(1, &fb->z_mask_handle);
  if (fb->vao != NULL) gl_vert_arr_destroy(fb->vao);
  if (fb->vbo != NULL) gl_buf_obj_destroy(fb->vbo);
  glDeleteFramebuffers(1, &fb->fb_handle);
  mem_destroy(fb);
}

void gl_framebuf_bind(const framebuf_t* fb)
{
  uint32_t handle = 0;
  if (fb != NULL) handle = fb->fb_handle;
  glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void gl_framebuf_resize(framebuf_t* fb, vec2i_t size)
{
  if (fb->color_handle != 0) glDeleteTextures(1, &fb->color_handle);
  if (fb->z_mask_handle != 0) glDeleteTextures(1, &fb->z_mask_handle);

  fb->size = size;

  glBindFramebuffer(GL_FRAMEBUFFER, fb->fb_handle);
  gen_texs(fb);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    log_warning(
      "framebuffer %d resizing could not be completed",
      fb->fb_handle
    );
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gl_framebuf_draw(
  const renderer_t* r, framebuf_t* fb, vec2i_t start, vec2i_t end)
{
  if (fb->vbo == NULL || fb->vao == NULL) {
    log_warning(
      "cannot draw framebuffer %d because it is not drawable",
      fb->fb_handle
    );
    return;
  }

  float vertices[] = {
    // pos                            uv     color
    start.x,         start.y,         0, 0,  1, 1, 1, 1, // tl
    start.x + end.x, start.y,         1, 0,  1, 1, 1, 1, // tr
    start.x,         start.y + end.y, 0, 1,  1, 1, 1, 1, // bl
    
    start.x + end.x, start.y,         1, 0,  1, 1, 1, 1, // tr
    start.x + end.x, start.y + end.y, 1, 1,  1, 1, 1, 1, // br
    start.x,         start.y + end.y, 0, 1,  1, 1, 1, 1, // bl
  };

  gl_buf_obj_set_dat(fb->vbo, vertices, sizeof(vertices), draw_dynamic);

  gl_set_active_shader(r, shader_2d);
  mat4_t i;
  mat4_identity(i);
  gl_setup_2d(r, shader_2d, fb->color_handle, i);
  gl_vert_arr_draw(fb->vao, 0, 6, idx_triangles);

  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_2D, fb->color_handle);
  //
  // gl_shader_bind(framebuffer_draw_shader);
  // gl_shader_send_int(framebuffer_draw_shader, "tex0", 0);
  // gl_vert_arr_draw(fb->vao, 0, 6, idx_triangles);
}
