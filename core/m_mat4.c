#include "m_mat4.h"

#include "m_math.h"

void mat4_identity(mat4_t mat)
{
  memset(mat, 0, sizeof(mat4_t));
  mat[0] = 1;
  mat[5] = 1;
  mat[10] = 1;
  mat[15] = 1;
}

// TODO: make dis use simd (SCARY)
void mat4_mult(mat4_t out, mat4_t a, mat4_t b)
{
  // 0  4  8  12
  // 1  5  9  13
  // 2  6  10 14
  // 3  7  11 15

  out[0]  = a[0] * b[0]  + a[4] * b[1]  + a[8]  * b[2]  + a[12] * b[3];
  out[1]  = a[1] * b[0]  + a[5] * b[1]  + a[9]  * b[2]  + a[13] * b[3];
  out[2]  = a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3];
  out[3]  = a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3];

  out[4]  = a[0] * b[4]  + a[4] * b[5]  + a[8]  * b[6]  + a[12] * b[7];
  out[5]  = a[1] * b[4]  + a[5] * b[5]  + a[9]  * b[6]  + a[13] * b[7];
  out[6]  = a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7];
  out[7]  = a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7];

  out[8]  = a[0] * b[8]  + a[4] * b[9]  + a[8]  * b[10] + a[12] * b[11];
  out[9]  = a[1] * b[8]  + a[5] * b[9]  + a[9]  * b[10] + a[13] * b[11];
  out[10] = a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11];
  out[11] = a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11];

  out[12] = a[0] * b[12] + a[4] * b[13] + a[8]  * b[14] + a[12] * b[15];
  out[13] = a[1] * b[12] + a[5] * b[13] + a[9]  * b[14] + a[13] * b[15];
  out[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
  out[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
}

vec4f_t mat4_mult_v4(const mat4_t m, vec4f_t v)
{
  vec4f_t o;
  o.x = m[0] * v.x + m[4] * v.y + m[8]  * v.z + m[12] * v.w;
  o.y = m[1] * v.x + m[5] * v.y + m[9]  * v.z + m[13] * v.w;
  o.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
  o.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
  return o;
}

void mat4_inverse(mat4_t out, const mat4_t m)
{
  // I really hate writing matrix code myself :)
  // https://github.com/love2d/love/blob/main/src/common/Matrix.cpp#L318
  out[0] =
    m[5]  * m[10] * m[15] -
    m[5]  * m[11] * m[14] -
    m[9]  * m[6]  * m[15] +
    m[9]  * m[7]  * m[14] +
    m[13] * m[6]  * m[11] -
    m[13] * m[7]  * m[10];

	out[4] =
    -m[4]  * m[10] * m[15] +
    m[4]  * m[11] * m[14] +
    m[8]  * m[6]  * m[15] -
    m[8]  * m[7]  * m[14] -
    m[12] * m[6]  * m[11] +
    m[12] * m[7]  * m[10];

  out[8] =
    m[4]  * m[9]  * m[15] -
    m[4]  * m[11] * m[13] -
    m[8]  * m[5]  * m[15] +
    m[8]  * m[7]  * m[13] +
    m[12] * m[5]  * m[11] -
    m[12] * m[7]  * m[9];

  out[12] =
    -m[4]  * m[9]  * m[14] +
    m[4]  * m[10] * m[13] +
    m[8]  * m[5]  * m[14] -
    m[8]  * m[6]  * m[13] -
    m[12] * m[5]  * m[10] +
    m[12] * m[6]  * m[9];

  out[1] =
    -m[1]  * m[10] * m[15] +
    m[1]  * m[11] * m[14] +
    m[9]  * m[2]  * m[15] -
    m[9]  * m[3]  * m[14] -
    m[13] * m[2]  * m[11] +
    m[13] * m[3]  * m[10];

  out[5] =
    m[0]  * m[10] * m[15] -
    m[0]  * m[11] * m[14] -
    m[8]  * m[2]  * m[15] +
    m[8]  * m[3]  * m[14] +
    m[12] * m[2]  * m[11] -
    m[12] * m[3]  * m[10];

  out[9] =
    -m[0]  * m[9]  * m[15] +
    m[0]  * m[11] * m[13] +
    m[8]  * m[1]  * m[15] -
    m[8]  * m[3]  * m[13] -
    m[12] * m[1]  * m[11] +
    m[12] * m[3]  * m[9];

  out[13] =
    m[0]  * m[9]  * m[14] -
    m[0]  * m[10] * m[13] -
    m[8]  * m[1]  * m[14] +
    m[8]  * m[2]  * m[13] +
    m[12] * m[1]  * m[10] -
    m[12] * m[2]  * m[9];

  out[2] =
    m[1]  * m[6] * m[15] -
    m[1]  * m[7] * m[14] -
    m[5]  * m[2] * m[15] +
    m[5]  * m[3] * m[14] +
    m[13] * m[2] * m[7] -
    m[13] * m[3] * m[6];

  out[6] =
    -m[0]  * m[6] * m[15] +
    m[0]  * m[7] * m[14] +
    m[4]  * m[2] * m[15] -
    m[4]  * m[3] * m[14] -
    m[12] * m[2] * m[7] +
    m[12] * m[3] * m[6];

  out[10] =
    m[0]  * m[5] * m[15] -
    m[0]  * m[7] * m[13] -
    m[4]  * m[1] * m[15] +
    m[4]  * m[3] * m[13] +
    m[12] * m[1] * m[7] -
    m[12] * m[3] * m[5];

  out[14] =
    -m[0]  * m[5] * m[14] +
    m[0]  * m[6] * m[13] +
    m[4]  * m[1] * m[14] -
    m[4]  * m[2] * m[13] -
    m[12] * m[1] * m[6] +
    m[12] * m[2] * m[5];

  out[3] =
    -m[1] * m[6] * m[11] +
    m[1] * m[7] * m[10] +
    m[5] * m[2] * m[11] -
    m[5] * m[3] * m[10] -
    m[9] * m[2] * m[7] +
    m[9] * m[3] * m[6];

  out[7] =
    m[0] * m[6] * m[11] -
    m[0] * m[7] * m[10] -
    m[4] * m[2] * m[11] +
    m[4] * m[3] * m[10] +
    m[8] * m[2] * m[7] -
    m[8] * m[3] * m[6];

  out[11] =
    -m[0] * m[5] * m[11] +
    m[0] * m[7] * m[9] +
    m[4] * m[1] * m[11] -
    m[4] * m[3] * m[9] -
    m[8] * m[1] * m[7] +
    m[8] * m[3] * m[5];

  out[15] =
    m[0] * m[5] * m[10] -
    m[0] * m[6] * m[9] -
    m[4] * m[1] * m[10] +
    m[4] * m[2] * m[9] +
    m[8] * m[1] * m[6] -
    m[8] * m[2] * m[5];

	float det = m[0] * out[0] + m[1] * out[4] + m[2] * out[8] + m[3] * out[12];

	float invdet = 1.0f / det;

	for (int i = 0; i < 16; i++)
		out[i] *= invdet;
}

void mat4_translate(mat4_t mat, vec3f_t trans)
{
  mat4_identity(mat);
  mat[12] = trans.x;
  mat[13] = trans.y;
  mat[14] = trans.z;
}

void mat4_scale(mat4_t mat, vec3f_t scale)
{
  mat4_identity(mat);
  mat[0] = scale.x;
  mat[5] = scale.y;
  mat[10] = scale.z;
}

// TODO: maybe make this rotate about an axis?
void mat4_rotate(mat4_t mat, vec3f_t rotation)
{
  float cosx = cosf(rotation.x);
  float sinx = sinf(rotation.x);

  float cosy = cosf(rotation.y);
  float siny = sinf(rotation.y);

  float cosz = cosf(rotation.z);
  float sinz = sinf(rotation.z);

  mat[0] = cosy * cosz;
  mat[1] = cosy * sinz;
  mat[2] = -siny;
  mat[3] = 0;
  mat[4] = cosz * sinx * siny - cosx * sinz;
  mat[5] = cosx * cosz + sinx * siny * sinz;
  mat[6] = cosy * sinx;
  mat[7] = 0;
  mat[8] = cosx * cosz * siny + sinx * sinz;
  mat[9] = -cosz * sinx + cosx * siny * sinz;
  mat[10] = cosx * cosy;
  mat[11] = 0;
  mat[12] = 0;
  mat[13] = 0;
  mat[14] = 0;
  mat[15] = 1;
}

void mat4_perspective(mat4_t mat, float yfov, float a, float n, float f)
{
  // thank you love2d for letting me continue to not understand how this works
  // https://github.com/love2d/love/blob/main/src/common/Matrix.cpp#L459
  memset(mat, 0, sizeof(mat4_t));

  float cotan = 1.0 / tanf(yfov * deg2rad * 0.5);
	mat[0] = cotan / a;
	mat[5] = cotan;
	mat[10] = (f + n) / (n - f);
	mat[11] = -1.0;
	mat[14] = 2.0 * n * f / (n - f);
}

void mat4_ortho(
  mat4_t mat,
  float l, float r,
  float b, float t,
  float n, float f)
{
  // https://github.com/love2d/love/blob/main/src/common/Matrix.cpp#L444
  mat4_identity(mat);

  mat[0] = 2.0 / (r - l);
	mat[5] = 2.0 / (t - b);
	mat[10] = -2.0 / (f - n);

	mat[12] = -(r + l) / (r - l);
	mat[13] = -(t + b) / (t - b);
	mat[14] = -(f + n) / (f - n);
}
