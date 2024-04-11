#ifndef SRC_CAMERA_CAMERA_H_
#define SRC_CAMERA_CAMERA_H_

#include <glm/mat4x4.hpp>

class Camera {
public:
  Camera();
  glm::mat4 getViewMatrix() const;
  void setEyePosition(glm::vec3);

  void moveLeft(float speed);
  void moveRight(float speed);
  void moveForward(float speed);
  void moveBack(float speed);

private:
  glm::vec3 eyePos;
  glm::vec3 lookDir;
  glm::vec3 up;
};


#endif // !SRC_CAMERA_CAMERA_H_
