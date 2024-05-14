#version 450

layout(location = 0) in vec3 v_in_position;
layout(location = 1) in vec3 v_in_normal;

uniform mat4 v_u_mvp;

void main() {
  gl_Position = v_u_mvp * vec4(v_in_position, 1.0f);
}