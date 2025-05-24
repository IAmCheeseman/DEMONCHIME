#include "math_transform.h"

#include "mathx.h"

void transform_to_matrix(transform_t trans, mat4_t mat)
{
  mat4_t translation;
  mat4_translate(translation, trans.pos);

  mat4_t rotation;
  mat4_rotate(rotation, trans.rot);

  mat4_t scale;
  mat4_scale(scale, trans.scale);

  mat4_t inter;
  // mat4Multiply(inter, translation, rotation);

  // translation * rotation * scale
  mat4_mult(inter, rotation, scale);
  mat4_mult(mat, translation, inter);
}
