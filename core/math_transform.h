#ifndef __bse_math_transform__
#define __bse_math_transform__

#include "math_vec3f.h"
#include "math_mat4.h"

typedef struct transform_s
{
  vec3f_t pos;
  vec3f_t rot;
  vec3f_t scale;
} transform_t;

void transform_to_matrix(transform_t trans, mat4_t mat);

#endif
