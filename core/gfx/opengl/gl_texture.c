#include "gfx/texture.h"

#include <glad/glad.h>

#include "gl_type_conv.h"
#include "mem.h"
#include "gl_texture.h"

struct Texture gl_TextureLoadFromImg(struct Image* img)
{
  struct Texture tex;

  tex.size = img->size;
  tex.format = img->format;

  uint32_t gl_format = ImageFormatToOpenGl(img->format);

  uint32_t handle;

  glGenTextures(1, &handle);
  glBindTexture(GL_TEXTURE_2D, handle);
  glTexImage2D(
    GL_TEXTURE_2D,
    0, gl_format, tex.size.x, tex.size.y,
    0, gl_format, GL_UNSIGNED_BYTE, img->data);

  LogDebug("loaded texture %d", handle);

  uint32_t* handle_ptr = (uint32_t*)Alloc(sizeof(uint32_t));
  *handle_ptr = handle;

  tex.handle = handle_ptr;

  gl_TextureSetFilter(&tex, TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
  gl_TextureSetWrap(&tex, TEXTURE_WRAP_REPEAT, TEXTURE_WRAP_REPEAT);

  return tex;
}

void gl_TextureDestroy(struct Texture* tex)
{
  uint32_t handle = *((uint32_t*)tex->handle);
  glDeleteTextures(1, &handle);
  LogDebug("destroyed texture %d", handle);
  Destroy(tex->handle);
}

void gl_TextureBind(struct Texture* tex, uint8_t slot)
{
  uint32_t handle = 0;
  if (tex != NULL) handle = *((uint32_t*)tex->handle);
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, handle);
}

void gl_TextureGenerateMipmaps(struct Texture* tex)
{
  switch (tex->min_filter) {
    case TEXTURE_FILTER_NEAREST:
    case TEXTURE_FILTER_LINEAR:
      break;
    default:
      break;
  }

  if (
    tex->min_filter == TEXTURE_FILTER_NEAREST ||
    tex->min_filter == TEXTURE_FILTER_LINEAR
  ) {
    LogWarning(
      "min texture filter for texture %d is not a mipmap filter; "
      "generated mipmaps will not make a difference",
      *((uint32_t*)tex->handle));
  }

  gl_TextureBind(tex, 0);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void gl_TextureSetFilter(
  struct Texture* tex,
  enum TextureFilter min,
  enum TextureFilter mag)
{
  tex->min_filter = min;
  tex->mag_filter = mag;

  if (
    mag != TEXTURE_FILTER_NEAREST &&
    mag != TEXTURE_FILTER_LINEAR
  ) {
    LogWarning(
      "mag texture filter for texture %d is a mipmap filter and will not make "
      "a difference",
      *((uint32_t*)tex->handle));
  }
  
  gl_TextureBind(tex, 0);
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MIN_FILTER,
    TextureFilterToOpenGl(min)
  );
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_MAG_FILTER,
    TextureFilterToOpenGl(mag)
  );
}

void gl_TextureSetWrap(
  struct Texture* tex,
  enum TextureWrap x_wrap,
  enum TextureWrap y_wrap)
{
  tex->x_wrap = x_wrap;
  tex->y_wrap = y_wrap;
  
  gl_TextureBind(tex, 0);
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_WRAP_S,
    TextureWrapToOpenGl(x_wrap)
  );
  glTexParameteri(
    GL_TEXTURE_2D,
    GL_TEXTURE_WRAP_T,
    TextureWrapToOpenGl(y_wrap)
  );
}
