#version 450 core

layout(location = 0) in vec3 v_in_position;

// Ä£ÐÍmodel * view * projection
uniform mat4 v_u_mvp;

void main() {
  vec4 vInPos = vec4(v_in_position, 1.0f);

  gl_Position = v_u_mvp * vInPos;
}