#ifndef SRC_SHADER_SHADERPARAM_H_
#define SRC_SHADER_SHADERPARAM_H_

// Third party
#include <gl3w/GL/gl3w.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

/// <summary>
/// �ƹ�shader���������
/// </summary>
struct LightShaderParam {
  glm::vec3 position;
  glm::vec3 toward;
  GLfloat angle;
};

/// <summary>
/// ֱ���shader�������
/// </summary>
struct DirectionalLightShaderParam {
  glm::vec3 direction;
};

/// <summary>
/// �۹��shader���������
/// </summary>
struct SpotLightShaderParam {
  glm::vec3 position;
  glm::vec3 toward;
  GLfloat angle;
};

/// <summary>
/// ģ��shader���������
/// </summary>
struct ModelShaderParam {
  glm::mat4 mvp;
  glm::mat4 mv;
  glm::mat3 mvNormal;
  /* �ڹ�Դ��ͼ�µ�mvp */
  glm::mat4 mvpLight;
  glm::vec3 color;
  GLuint shadowSampleUnitId;
};

/// <summary>
/// ��shader���������
/// </summary>
struct SimpleShaderParam {
  glm::mat4 mvp;
};

#endif // !SRC_SHADER_SHADERPARAM_H_

