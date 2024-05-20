#include "Light.h"

#include <glm/ext/matrix_transform.hpp>

glm::mat4 SpotLight::getMatrix() const{
  glm::vec3 position = transform.position;
  glm::vec3 direction = getLookDir();
  glm::vec3 lookPoint = position + direction;
  return glm::lookAt(position, lookPoint, glm::vec3(0, 1, 0));
}

glm::vec3 SpotLight::getLookDir() const {
  glm::mat4 matrix = transform.getMatrix();
  glm::vec3 lookFromPointNew = matrix * glm::vec4(lookFromPoint, 1.0f);
  glm::vec3 lookToPointNew = matrix * glm::vec4(lookToPoint, 1.0f);

  glm::vec3 lookDirection = lookToPointNew - lookFromPointNew;
  lookDirection = glm::normalize(lookDirection);
  return lookDirection;
}

glm::vec3 DirectionalLight::getDirection() const
{
  glm::mat4 matrix = transform.getMatrix();
  glm::vec3 fromNew = matrix * glm::vec4(from, 1.0);
  glm::vec3 toNew = matrix * glm::vec4(to, 1.0);

  glm::vec3 res = fromNew - toNew;
  res = glm::normalize(res);
  return res;
}
