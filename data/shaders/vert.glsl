#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

//out vec4 fColor;

//uniform float uOffsetX; 
//uniform float uOffsetY;
uniform mat4 uMVP;
void main() {
  gl_Position = uMVP * vec4(vPosition, 1.0f);
//  fColor = vec4(vColor, 1.0);
}