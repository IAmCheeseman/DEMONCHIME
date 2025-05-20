#include "gl_buffer_object.h"

#include <glad/glad.h>

#include "mem.h"
#include "gl_type_conv.h"

struct BufferObject* gl_BufferObjectCreate(enum BufferObjectType type)
{
  struct BufferObject* buf =
    (struct BufferObject*)Alloc(sizeof(struct BufferObject));
  buf->type = type;
  glGenBuffers(1, &buf->handle);
  LogDebug("created buffer object %d", buf->handle);
  return buf;
}

void gl_BufferObjectDestroy(struct BufferObject* buf)
{
  glDeleteBuffers(1, &buf->handle);
  LogDebug("destroyed buffer object %d", buf->handle);
  Destroy(buf);
}

void gl_BufferObjectBind(struct BufferObject* buf)
{
  glBindBuffer(BufferObjectTypeToOpenGl(buf->type), buf->handle);
}

void gl_BufferObjectUnbind(enum BufferObjectType type)
{
  glBindBuffer(BufferObjectTypeToOpenGl(type), 0);
}

void gl_BufferObjectSet(
  struct BufferObject* buf,
  void* data,
  size_t size,
  enum DrawMode mode)
{
  uint32_t gl_mode = DrawModeToOpenGl(mode);
  uint32_t type = BufferObjectTypeToOpenGl(buf->type);
  glBindBuffer(type, buf->handle);
  glBufferData(type, size, data, gl_mode);
  glBindBuffer(type, 0);
}
