#version 450

layout(location = 0) out vec4 f_out_color;

// ģ�Ͳ�����ɫ
uniform vec3 f_u_color;

// light type, 0:direction, 1:spotlight, 2:pointlight 3:arealight
uniform int f_u_light_type;

// direction light param
uniform vec3 f_u_light_directional_direction;

// spot light param
uniform vec3 f_u_light_position;
uniform vec3 f_u_light_toward;
uniform float f_u_light_angle;

// shadow ���������Ԫ
uniform sampler2D f_u_sample_shadow;

in vec3 v_out_position_view;
in vec3 v_out_normal_view;

// �ڹ�Դ��ͼ�µ�λ������
in vec4 v_out_position_light;

// ���ʲ���
vec3 u_ka = f_u_color;
vec3 u_kd = f_u_color;
vec3 u_ks = vec3(1.0f, 1.0f, 1.0f);
float u_alpha = 64;

// intensity_diffuse:0.55, intensity_ambient: 0.25, intensitySpecular: 0.8;

float intensityDiffuse = 0.55f;
float intensitySpecular = 0.8f;
float intensityAmbient =  0.25f;

bool isLightInSpotAngle(vec3 dirLight, vec3 toward, float angle) {
  bool res = false;
  float angleDirection = acos(dot(-dirLight, toward));
  if (angleDirection * 2 < angle) { //TODO...���ȴ���
    res = true;
  }
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

vec3 blinnPhongShading() {
  vec3 dirLight = vec3(0);
  // �жϹ�Դ����
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

  // ��Ӱ����
  bool isInShadow = false;
  vec3 positionInLight = v_out_position_light.xyz / v_out_position_light.w;
  if (texture(f_u_sample_shadow, positionInLight.xy).r < positionInLight.z) {
    // ����Ӱ��
	isInShadow = true;
  }
  
  if (f_u_light_type == 0 && !isInShadow) {
    // direction light
	 // 1.������
    colorDiffuse = computeDiffuse(u_kd, cosinTheta, intensityDiffuse);
    // 2.�߹�
    colorSpecular = computeSpecular(u_ks, cosinTheta, cosinPhai, u_alpha, intensitySpecular);
  }
  else if (f_u_light_type == 1 && !isInShadow) {
    // spot light
    // �ж��Ƿ񳬳��۹�Ƶƹ�Ƕ�
	bool isInSpotAngle = isLightInSpotAngle(dirLight, f_u_light_toward, f_u_light_angle);
	if (isInSpotAngle) {
    // 1.������
    colorDiffuse = computeDiffuse(u_kd, cosinTheta, intensityDiffuse);
    // 2.�߹�
    colorSpecular = computeSpecular(u_ks, cosinTheta, cosinPhai, u_alpha, intensitySpecular);
	}
  }
 
  // 3.������
  colorAmbient = computeAmbient(u_ka, intensityAmbient);
  // ������ɫ
  vec3 colorFinal = colorDiffuse + colorSpecular + colorAmbient;
  return colorFinal;
}

void main() {
  // Blinn-phong shading
  vec3 finalColor = blinnPhongShading();
  f_out_color = vec4(finalColor, 1.0f);
}