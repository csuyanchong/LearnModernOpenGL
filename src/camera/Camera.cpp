#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

Camera::Camera() {
  eyePos = glm::vec3(0, 0, 0);
  lookDir = glm::vec3(0, 0, -1);
  up = glm::vec3(0, 1.0f, 0);
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(eyePos, lookDir, up);
}

void Camera::moveLeft(float speed) {
  eyePos.x -= speed;
}

void Camera::moveRight(float speed) {
  eyePos.x += speed;
}

void Camera::moveForward(float speed) {
  eyePos.z -= speed;
}

void Camera::moveBack(float speed) {
  eyePos.z += speed;
}
