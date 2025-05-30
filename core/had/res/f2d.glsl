#version 330 core

in vec2 f_uv;
in vec4 f_color;

out vec4 out_color;

uniform sampler2D tex;

void main()
{
  out_color = texture(tex, f_uv) * f_color;
}
