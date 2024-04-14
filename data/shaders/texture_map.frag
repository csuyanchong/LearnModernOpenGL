#version 450 core

in vec2 v_out_texcoord;

uniform sampler2D f_u_sample2d;

out vec3 color;

void main() {
  color = texture(f_u_sample2d, v_out_texcoord).rgb;
// color = vec4(1.0f, 0, 0);
}