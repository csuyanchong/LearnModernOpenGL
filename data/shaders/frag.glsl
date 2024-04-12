#version 450 core

layout (location = 0) out vec4 color;

in vec3 vOutPosView;
in vec3 vOutNormal;

uniform vec3 uDirLight;

uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uAlpha;

void main() {
  float cosinTheta = dot(vOutNormal, uDirLight);

  vec3 dirView = -vOutPosView;
  vec3 dirHalf = normalize(uDirLight + dirView);

  float cosinPhai = dot(dirHalf, vOutNormal);

  // Blinn-phong shading
  vec3 colorCompute = clamp(cosinTheta, 0, 1) * (uKd + uKs * pow(cosinPhai, uAlpha) / cosinTheta) + uKa;
  // final color
  //  color = vec4(1.0f, 0, 0, 1.0f);
  //  color = vec4(colorCompute, 1.0f);
 
  // 1. 漫反射
  vec3 colorDiffuse = uKd * clamp(cosinTheta, 0, 1);
  // 2.高光

  // 3.环境光
  vec3 colorAmbient = uKa;

  // 最终颜色
  vec3 colorFinal = colorDiffuse + colorAmbient;

  color = vec4(colorFinal, 1.0f);
}