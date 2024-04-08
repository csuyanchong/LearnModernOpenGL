#version 450 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec3 vColor;

out vec4 fColor;

uniform float uOffsetX; 
uniform float uOffsetY;

void main() {
  gl_Position = vec4(vPosition.x + uOffsetX, vPosition.y + uOffsetY, 0, 1.0);
  fColor = vec4(vColor, 1.0);
}