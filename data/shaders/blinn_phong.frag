#version 450 core

layout(location = 0) out vec4 color;

in vec3 v_out_posView;
in vec3 v_out_normal;

uniform vec3 u_dirLight;

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_alpha;

// intensity_diffuse:0.55, intensity_ambient: 0.25, intensitySpecular: 0.8;

float intensityDiffuse = 0.55f;
float intensitySpecular = 0.8f;
float intensityAmbient =  0.25f;

in vec2 v_out_texture_coord;

vec3 linearToSRGB(vec3 color) {
  vec3 res = pow(color, vec3 (1 / 2.2f));
  return res;
}

vec3 computeDiffuse(vec3 _kd, float _cosTheta, float _intensity) {
  vec3 res = _kd * max(0, _cosTheta)  * _intensity;
  return res;
}

vec3 computeSpecular(vec3 _ks, float _cosTheta, float _cosPhai, float _alpha, float _intensity) {
  vec3 res = vec3(0, 0, 0);
  if (_cosTheta > 0) {
    res = _ks * pow(_cosPhai, _alpha) * _intensity;
  }
  return res;
}

vec3 computeAmbient(vec3 _ka, float _intensity) {
  vec3 res = _ka *_intensity;
  return res;
}

void main() {
  vec3 dirNormal = normalize(v_out_normal);

  vec3 dirLight = normalize(u_dirLight);

  float cosinTheta = dot(dirNormal, dirLight);

  vec3 dirView = -v_out_posView;

  dirView = normalize(dirView);

  vec3 dirHalf = normalize(dirLight + dirView);

  float cosinPhai = dot(dirHalf, dirNormal);
  
  // Blinn-phong shading

  // 1.漫反射
  vec3 colorDiffuse = computeDiffuse(u_kd, cosinTheta, intensityDiffuse);
  // 2.高光
  vec3 colorSpecular = computeSpecular(u_ks, cosinTheta, cosinPhai, u_alpha, intensitySpecular);
  // 3.环境光
  vec3 colorAmbient = computeAmbient(u_ka, intensityAmbient);

  // 最终颜色
  vec3 colorFinal = colorDiffuse + colorSpecular + colorAmbient;

  // gamma校正
  colorFinal = linearToSRGB(colorFinal);
  color = vec4(colorFinal, 1.0f);
}