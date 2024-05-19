#version 450

layout(location = 0) in vec3 v_in_position;
layout(location = 1) in vec3 v_in_normal;

// 模型model * view * projection
uniform mat4 v_u_mvp;
// 模型model * view
uniform mat4 v_u_mv;
// 法线model * view
uniform mat3 v_u_mv_normal;

out vec3 v_out_position_view;
out vec3 v_out_normal_view;

void main() {
  vec4 vInPos = vec4(v_in_position, 1.0f);

  gl_Position = v_u_mvp * vInPos;
  
  vec4 vPosView = v_u_mv * vInPos;

  v_out_position_view = vec3(vPosView);

  v_out_normal_view = v_u_mv_normal * v_in_normal;
  v_out_normal_view = normalize(v_out_normal_view);
}