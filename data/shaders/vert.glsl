#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 uMV;
uniform mat4 uMVP;
uniform mat3 uMVNormal;

out vec3 vOutPosView;
out vec3 vOutNormal;

void main() {
  vec4 vOutPosViewCompute = uMV * vec4(vPosition, 1.0f);
  vOutPosView = normalize(vec3(vOutPosViewCompute));

  vOutNormal = normalize(uMVNormal * vNormal);
  gl_Position = uMVP * vec4(vPosition, 1.0f);
}