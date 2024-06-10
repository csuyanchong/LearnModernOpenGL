#version 450 core

layout(location = 0) in vec3 vPosition;

uniform mat4 v_u_mvp;

void main() {
  vec4 position = vec4(vPosition, 1.0f);
  gl_Position = v_u_mvp * position;
}