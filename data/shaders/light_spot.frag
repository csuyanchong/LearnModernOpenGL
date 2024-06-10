#version 450 core

layout(location = 0) out vec4 color;

uniform vec3 f_u_color;

void main() {
  color = vec4(f_u_color, 1.0f);
}