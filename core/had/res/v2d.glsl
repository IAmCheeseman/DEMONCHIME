#version 330 core

layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec2 v_uv;
layout (location = 2) in vec4 v_color;

out vec2 f_uv;
out vec4 f_color;

uniform mat4 p;

void main()
{
  gl_Position = p * vec4(v_pos, 0.0, 1.0);
  f_uv = v_uv;
  f_color = v_color;
}
