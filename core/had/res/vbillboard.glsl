#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_uv;
layout (location = 2) in vec4 v_color;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

uniform vec2 tex_size;
uniform float scale;

out vec3 f_pos;
out vec3 f_normal;
out vec2 f_uv;
out vec4 f_color;

void main()
{
  vec3 right = vec3(v[0][0], v[1][0], v[2][0]);
  vec3 up = vec3(0, 1, 0);
  vec3 pos = (vec4(0, 0, 0, 1) * m).xyz
    + right * v_pos.x * tex_size.x * scale
    + up * v_pos.y * tex_size.y * scale;
  gl_Position = p * v * m * vec4(pos, 1.0);

  mat3 normal_m = transpose(inverse(mat3(m)));
  vec3 normal = normalize(vec3(1, 0, 1) + right);

  f_pos = pos;
  f_normal = normal;
  f_uv = v_uv;
  f_color = v_color;
}
