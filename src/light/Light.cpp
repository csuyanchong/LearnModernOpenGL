#include "Light.h"

SpotLight::SpotLight() {
  position = glm::vec3(0);
  toward = glm::vec3(0, -1.0, 0);
  angle = 45.0f;
}
