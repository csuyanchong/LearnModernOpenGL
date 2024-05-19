#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

//Camera::Camera() {
//  eyePos = glm::vec3(0, 0, 0);
//  lookDir = glm::vec3(0, 0, -1);
//  up = glm::vec3(0, 1.0f, 0);
//}

//Camera::Camera(glm::vec3 _pos, glm::vec3 _look, glm::vec3 _up) {
//  eyePos = _pos;
//  lookDir = _look;
//  up = _up;
//}

glm::mat4 Camera::getViewMatrix() const {
  // 没有场景图，所有position是世界坐标。
  glm::vec3 eyePos = transform.position;
  glm::vec3 lookDir = getLookDirection();
  return glm::lookAt(eyePos, lookDir, up);
}

glm::vec3 Camera::getLookDirection() const {
  glm::mat4 matrix = transform.getMatrix();
  glm::vec3 fromNew = matrix * glm::vec4(from, 1.0);
  glm::vec3 toNew = matrix * glm::vec4(to, 1.0);

  glm::vec3 lookDir = toNew - fromNew;
  lookDir = glm::normalize(lookDir);
  return lookDir;
}

//void Camera::setEyePosition(glm::vec3 _pos) {
//  eyePos = _pos;
//}
//
//void Camera::setLookDirection(glm::vec3 _look) {
//  lookDir = _look;
//}
//
//void Camera::setUpDirection(glm::vec3 _up) {
//  up = _up;
//}
//
//void Camera::moveLeft(float speed) {
//  eyePos.x -= speed;
//}
//
//void Camera::moveRight(float speed) {
//  eyePos.x += speed;
//}
//
//void Camera::moveForward(float speed) {
//  eyePos.z -= speed;
//}
//
//void Camera::moveBack(float speed) {
//  eyePos.z += speed;
//}
