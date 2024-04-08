#version 450 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec3 vColor;

out vec4 fColor;

void main() {
  gl_Position = vec4(vPosition, 0, 1.0);
  fColor = vec4(vColor, 1.0);
}