#include "gl_vertex_array.h"

#include <glad/glad.h>

#include "mem.h"
#include "gl_type_conv.h"
#include "gl_buffer_object.h"

struct VertexArray* gl_VertexArrayCreate(const struct VertexFormat* fmt)
{
  struct VertexArray* varr =
    (struct VertexArray*)Alloc(sizeof(struct VertexArray));

  glGenVertexArrays(1, &varr->handle);
  LogDebug("created vertex array %d", varr->handle);

  // set up vertex format 
  glBindVertexArray(varr->handle);

  size_t offset = 0;
  for (size_t i = 0; i < fmt->attrib_count; i++) {
    const struct VertexAttrib* attrib = &fmt->attribs[i];
    uint8_t components = attrib->components;
    uint32_t gl_type = GfxDataTypeToOpenGl(attrib->type);

    glVertexAttribPointer(
      i,
      components, gl_type, GL_FALSE,
      fmt->stride, (void*)offset
    );
    glEnableVertexAttribArray(i);

    offset += GetGfxDataTypeSize(attrib->type) * components;
  }

  glBindVertexArray(0);

  return varr;
}

void gl_VertexArrayBind(struct VertexArray* varr)
{
  uint32_t handle = 0;
  if (varr != NULL) handle = varr->handle;
  glBindVertexArray(handle);
}

void gl_VertexArrayDestroy(struct VertexArray* varr)
{
  glDeleteVertexArrays(1, &varr->handle);
  LogDebug("destroyed vertex array %d", varr->handle);
  Destroy(varr);
}

void gl_VertexArrayDraw(
  struct VertexArray* varr,
  size_t start,
  size_t count,
  enum IndexMode index_mode)
{
  glBindVertexArray(varr->handle);
  glDrawArrays(IndexModeToOpenGl(index_mode), start, count);
  glBindVertexArray(0);
}

void gl_VertexArrayDrawIndexed(
  struct VertexArray* varr,
  struct BufferObject* ebo,
  size_t count,
  enum GfxDataType type,
  enum IndexMode index_mode)
{
  if (ebo->type != BUFFER_INDEX)
    LogFatal(1, "expected index buffer for index drawing");

  glBindVertexArray(varr->handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->handle);

  glDrawElements(
    IndexModeToOpenGl(index_mode),
    count,
    GfxDataTypeToOpenGl(type),
    NULL
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
