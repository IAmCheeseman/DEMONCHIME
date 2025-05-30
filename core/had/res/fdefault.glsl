#version 330 core

uniform sampler2D tex;

out vec4 out_color;

in vec3 f_pos;
in vec3 f_normal;
in vec2 f_uv;
in vec4 f_color;

const float shadow_range = 1.0;

void main()
{
  vec4 texel = texture(tex, f_uv);

  vec3 light_dir = normalize(vec3(1, -2, 0));
  float shadow_dot = min(dot(f_normal, light_dir), 1.0);
  float shadow = (shadow_dot + 1.0) / 2.0;
  shadow = shadow_range * shadow + (1.0 - shadow_range);

  out_color = vec4(texel.rgb * shadow, texel.a) * f_color;
}
