#include "Light.h"

#include <glm/ext/matrix_transform.hpp>

SpotLight::SpotLight() {
  position = glm::vec3(0);
  lookPoint = glm::vec3(0, -1.0, 0);
  angle = 45.0f;
}

glm::mat4 SpotLight::getMatrix() const{
    return glm::lookAt(position, lookPoint, glm::vec3(0, 1, 0));
}

glm::vec3 SpotLight::getLookDir() const {
  glm::vec3 dir = lookPoint - position;
  dir = glm::normalize(dir);
  return dir;
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
