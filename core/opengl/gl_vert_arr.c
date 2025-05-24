#include "gl_vert_arr.h"

#include <glad/glad.h>

#include "c_mem.h"
#include "gl_type_conv.h"
#include "gl_buf_obj.h"

vert_arr_t* gl_vert_arr_create(const vert_fmt_t* fmt)
{
  vert_arr_t* varr =
    (vert_arr_t*)mem_alloc(sizeof(vert_arr_t));

  glGenVertexArrays(1, &varr->handle);
  log_debug("created vertex array %d", varr->handle);

  // set up vertex format 
  glBindVertexArray(varr->handle);

  size_t offset = 0;
  for (size_t i = 0; i < fmt->attrib_count; i++) {
    const vert_attr_t* attrib = &fmt->attribs[i];
    uint8_t components = attrib->components;
    uint32_t gl_type = data_type_to_gl(attrib->type);

    glVertexAttribPointer(
      i,
      components, gl_type, GL_FALSE,
      fmt->stride, (void*)offset
    );
    glEnableVertexAttribArray(i);

    offset += get_data_type_size(attrib->type) * components;
  }

  glBindVertexArray(0);

  return varr;
}

void gl_vert_arr_bind(const vert_arr_t* varr)
{
  uint32_t handle = 0;
  if (varr != NULL) handle = varr->handle;
  glBindVertexArray(handle);
}

void gl_vert_arr_destroy(vert_arr_t* varr)
{
  glDeleteVertexArrays(1, &varr->handle);
  log_debug("destroyed vertex array %d", varr->handle);
  mem_destroy(varr);
}

void gl_vert_arr_draw(
  const vert_arr_t* varr,
  size_t start,
  size_t count,
  idx_mode_t index_mode)
{
  glBindVertexArray(varr->handle);
  glDrawArrays(idx_mode_to_gl(index_mode), start, count);
  glBindVertexArray(0);
}

void gl_vert_arr_draw_idx(
  const vert_arr_t* varr,
  const buf_obj_t* ebo,
  size_t count,
  data_type_t type,
  idx_mode_t index_mode)
{
  if (ebo->type != buf_idx)
    log_warning(
      "expected index buffer for index drawing (VAO %d, BO %d)",
      varr->handle, ebo->handle);

  glBindVertexArray(varr->handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->handle);

  glDrawElements(
    idx_mode_to_gl(index_mode),
    count,
    data_type_to_gl(type),
    NULL
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
