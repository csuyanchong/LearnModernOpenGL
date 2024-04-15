#version 450 core

in vec2 v_out_texcoord;

uniform sampler2D f_u_sample2d_0;
uniform sampler2D f_u_sample2d_1;

layout (location = 0) out vec4 color;

void main() {
  color = texture(f_u_sample2d_0, v_out_texcoord) * texture(f_u_sample2d_1, v_out_texcoord);
}