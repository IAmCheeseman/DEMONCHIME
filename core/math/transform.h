#ifndef __bse_math_transform__
#define __bse_math_transform__

#include "vec3f.h"
#include "mat4.h"

typedef struct transform_s
{
  vec3f_t pos;
  vec3f_t rot;
  vec3f_t scale;
} transform_t;

void TransformToMatrix(transform_t trans, mat4_t mat);

#endif
