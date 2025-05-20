#include "transform.h"

#include "mat4.h"

void TransformToMatrix(transform_t trans, mat4_t mat)
{
  mat4_t translation;
  Mat4Translate(translation, trans.pos);

  mat4_t rotation;
  Mat4Rotate(rotation, trans.rot);

  mat4_t scale;
  Mat4Scale(scale, trans.scale);

  mat4_t inter;
  // mat4Multiply(inter, translation, rotation);

  // translation * rotation * scale
  Mat4Multiply(inter, rotation, scale);
  Mat4Multiply(mat, translation, inter);
}
