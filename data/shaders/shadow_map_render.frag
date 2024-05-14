#version 450

layout(location = 0) out vec4 f_out_color;

uniform vec3 f_u_color;

void main() {
  f_out_color = vec4(f_u_color, 1.0f);
}