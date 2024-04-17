#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTextureCoord;

uniform mat4 u_mv;
uniform mat4 u_mvp;
uniform mat3 u_normal;

out vec3 v_out_posView;
out vec3 v_out_normal;
out vec2 v_out_texture_coord;

void main() {
  // ����任��View�ռ�
  vec4 posView = u_mv * vec4(vPosition, 1.0f);
  v_out_posView = normalize(vec3(posView));
  // ���߱任��View�ռ�
  v_out_normal = normalize(u_normal * vNormal);
  // ����ͶӰλ��
  gl_Position = u_mvp * vec4(vPosition, 1.0f);
  // ����uv����
  v_out_texture_coord = vTextureCoord;
}