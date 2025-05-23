#ifndef __bse_math_mat4__
#define __bse_math_mat4__

#include <string.h>
#include <math.h>
#include "defs.h"
#include "vec3f.h"
#include "include.h"

typedef float mat4_t[16];

void mat4_identity(mat4_t mat);
void mat4_mult(mat4_t out, mat4_t a, mat4_t b);
void mat4_translate(mat4_t mat, vec3f_t trans);
void mat4_scale(mat4_t mat, vec3f_t scale);
void mat4_rotate(mat4_t mat, vec3f_t rotation);
void mat4_perspective(mat4_t mat, float yfov, float a, float n, float f);
void mat4_ortho(
  mat4_t mat,
  float l, float r,
  float b, float t,
  float n, float f);

#endif
