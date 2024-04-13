#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

Camera::Camera() {
  eyePos = glm::vec3(0, 0, 0);
  lookDir = glm::vec3(0, 0, -1);
  up = glm::vec3(0, 1.0f, 0);
}

Camera::Camera(glm::vec3 _pos, glm::vec3 _look, glm::vec3 _up) {
  eyePos = _pos;
  lookDir = _look;
  up = _up;
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(eyePos, lookDir, up);
}

void Camera::setEyePosition(glm::vec3 _pos) {
  eyePos = _pos;
}

void Camera::setLookDirection(glm::vec3 _look) {
  lookDir = _look;
}

void Camera::setUpDirection(glm::vec3 _up) {
  up = _up;
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
