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
  glm::vec3 lookDirection = getLookDirection();
  glm::vec3 lookPoint = eyePos + lookDirection;
  return glm::lookAt(eyePos, lookPoint, up);
}

glm::vec3 Camera::getLookDirection() const {
  glm::mat4 matrix = transform.getMatrix();
  glm::vec3 lookFromPointNew = matrix * glm::vec4(lookFromPoint, 1.0f);
  glm::vec3 lookToPointNew = matrix * glm::vec4(lookToPoint, 1.0f);

  glm::vec3 lookDirection = lookToPointNew - lookFromPointNew;
  lookDirection = glm::normalize(lookDirection);
  return lookDirection;
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
