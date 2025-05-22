#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;
layout (location = 3) in vec4 v_color;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

out vec3 f_pos;
out vec3 f_normal;
out vec2 f_uv;
out vec4 f_color;

void main()
{
  gl_Position = p * v * m * vec4(v_pos, 1.0);

  mat3 normal_m = transpose(inverse(mat3(m)));

  f_pos = v_pos;
  f_normal = normalize(normal_m * v_normal);
  f_uv = v_uv;
  f_color = v_color;
}
