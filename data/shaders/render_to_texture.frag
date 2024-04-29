#version 450 core

in vec2 v_out_texcoord;

uniform sampler2D f_u_sample2d_diffuse;

layout (location = 0) out vec4 color;

void main() {
  color = texture(f_u_sample2d_diffuse, v_out_texcoord);
}