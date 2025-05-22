#version 330 core

in vec2 f_uv;

out vec4 out_color;

uniform sampler2D fontmap;
uniform vec4 text_color;

void main()
{
  vec4 color = vec4(1.0, 1.0, 1.0, texture(fontmap, f_uv).r);
  out_color = color * text_color;
}
