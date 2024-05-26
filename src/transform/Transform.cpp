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

Transform& Transform::operator=(const Transform& other)
{
  position = other.position;
  euler = other.euler;
  scale = other.scale;
  return *this;
}

glm::mat4 Transform::getMatrix() const
{
  /*glm::eulerAngleYXZ(euler.y, euler.x, euler.z);
  
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, position);

  modelMatrix = glm::rotate(modelMatrix, glm::radians(euler.z), glm::vec3(0, 0, 1));
  modelMatrix = glm::rotate(modelMatrix, glm::radians(euler.y), glm::vec3(0, 1, 0));
  modelMatrix = glm::rotate(modelMatrix, glm::radians(euler.x), glm::vec3(1, 0, 0));

  modelMatrix = glm::scale(modelMatrix, scale);*/

  glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), position);

  glm::mat4 rotationMatrix = glm::eulerAngleYXZ(glm::radians(euler.y), glm::radians(euler.x), glm::radians(euler.z));

  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
 
  glm::mat4 modelMatrix = translateMatrix * rotationMatrix * scaleMatrix;

  return modelMatrix;
}
