#version 450 core

layout (location = 0) in vec3 v_in_position;
layout (location = 1) in vec3 v_in_normal;
layout (location = 2) in vec2 v_in_texcoord;

uniform mat4 v_u_mvp;

out vec2 v_out_texcoord;

void main() {
	gl_Position = v_u_mvp * vec4(v_in_position, 1.0f);
	v_out_texcoord = v_in_texcoord;
}