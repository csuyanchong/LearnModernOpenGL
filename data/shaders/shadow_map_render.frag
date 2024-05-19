#version 450

layout(location = 0) out vec4 f_out_color;

// 模型材质颜色
uniform vec3 f_u_color;

// light type, 0:direction, 1:spotlight, 2:pointlight 3:arealight
uniform int f_u_light_type;

// direction light param
uniform vec3 f_u_light_directional_direction;

// spot light param
uniform vec3 f_u_light_position;
uniform vec3 f_u_light_toward;
uniform float f_u_light_angle;

in vec3 v_out_position_view;
in vec3 v_out_normal_view;

// 材质参数
vec3 u_ka = f_u_color;
vec3 u_kd = f_u_color;
vec3 u_ks = vec3(1.0f, 1.0f, 1.0f);
float u_alpha = 64;

// intensity_diffuse:0.55, intensity_ambient: 0.25, intensitySpecular: 0.8;

float intensityDiffuse = 0.55f;
float intensitySpecular = 0.8f;
float intensityAmbient =  0.25f;

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

vec3 blinnPhongShading() {
  vec3 dirLight = vec3(0);
  // 判断光源类型
  if (f_u_light_type == 0) {
    // direction light
	dirLight = f_u_light_directional_direction;
  }
  else if (f_u_light_type == 1) {
    // spot light
    dirLight = f_u_light_position - v_out_position_view;
  }

  dirLight = normalize(dirLight);

  float cosinTheta = dot(dirLight, v_out_normal_view);

  vec3 dirView = normalize(-v_out_position_view);

  vec3 dirHalf = normalize(dirLight + dirView);

  float cosinPhai = dot(dirHalf, v_out_normal_view);

  // Blinn-phong shading

  vec3 colorDiffuse = vec3(0);
  vec3 colorSpecular = vec3(0);
  vec3 colorAmbient = vec3(0);

  // 判断是否超出聚光灯灯光角度
  bool isInSpotAngle = true;
  if (isInSpotAngle) {
    // 1.漫反射
    colorDiffuse = computeDiffuse(u_kd, cosinTheta, intensityDiffuse);
    // 2.高光
    colorSpecular = computeSpecular(u_ks, cosinTheta, cosinPhai, u_alpha, intensitySpecular);
  }
 
  // 3.环境光
  colorAmbient = computeAmbient(u_ka, intensityAmbient);
  // 最终颜色
  vec3 colorFinal = colorDiffuse + colorSpecular + colorAmbient;
  return colorFinal;
}

void main() {
  vec3 finalColor = blinnPhongShading();
  f_out_color = vec4(finalColor, 1.0f);
}