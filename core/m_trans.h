#ifndef __bse_math_transform__
#define __bse_math_transform__

#include "m_vec3f.h"
#include "m_mat4.h"

typedef struct trans
{
  vec3f_t pos;
  vec3f_t rot;
  vec3f_t scale;
} trans_t;

void trans_to_matrix(trans_t trans, mat4_t mat);

#endif
