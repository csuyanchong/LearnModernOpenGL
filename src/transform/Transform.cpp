#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/euler_angles.hpp>

Transform::Transform() {
  position = glm::vec3(0);
  euler = glm::vec3(0);
  scale = glm::vec3(1.0f);
}

glm::mat4 Transform::getMatrix() const
{
  //glm::mat4 modelMatrix = glm::mat4(1);
  //modelMatrix = glm::rotate(modelMatrix, 1.0f, glm::vec3(0, 1.0f, 0));
  glm::mat4 modelMatrix = glm::eulerAngleXYZ(euler.x, euler.y, euler.z) * glm::mat4(1);
  modelMatrix = glm::scale(modelMatrix, scale);
  modelMatrix = glm::translate(modelMatrix, position);
  return modelMatrix;
}
