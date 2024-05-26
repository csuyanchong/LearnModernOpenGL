#ifndef SRC_SHADER_SHADERPARAM_H_
#define SRC_SHADER_SHADERPARAM_H_

// Third party
#include <gl3w/GL/gl3w.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

/// <summary>
/// 灯光shader传入参数。
/// </summary>
struct LightShaderParam {
  glm::vec3 position;
  glm::vec3 toward;
  GLfloat angle;
};

/// <summary>
/// 直射光shader传入参数
/// </summary>
struct DirectionalLightShaderParam {
  glm::vec3 direction;
};

/// <summary>
/// 聚光灯shader传入参数。
/// </summary>
struct SpotLightShaderParam {
  glm::vec3 position;
  glm::vec3 toward;
  GLfloat angle;
};

/// <summary>
/// 模型shader传入参数。
/// </summary>
struct ModelShaderParam {
  glm::mat4 mvp;
  glm::mat4 mv;
  glm::mat3 mvNormal;
  /* 在光源视图下的mvp */
  glm::mat4 mvpLight;
  glm::vec3 color;
  GLuint shadowSampleUnitId;
};

/// <summary>
/// 简单shader传入参数。
/// </summary>
struct SimpleShaderParam {
  glm::mat4 mvp;
};

#endif // !SRC_SHADER_SHADERPARAM_H_

