#ifndef SRC_CAMERA_CAMERA_H_
#define SRC_CAMERA_CAMERA_H_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../transform/Transform.h"

class Camera {
public:
  Camera() : up(glm::vec3(0, 1.0f, 0)), lookFromPoint(glm::vec3(0)), lookToPoint(glm::vec3(0, 0, -1.0f)) {};
  //Camera(glm::vec3 _pos, glm::vec3 _look, glm::vec3 _up);

  glm::mat4 getViewMatrix() const;

  glm::vec3 getLookDirection() const;
  //void setEyePosition(glm::vec3 _pos);
  //void setLookDirection(glm::vec3 _look);
  //void setUpDirection(glm::vec3 _up);

  //void moveLeft(float speed);
  //void moveRight(float speed);
  //void moveForward(float speed);
  //void moveBack(float speed);

public:
  Transform transform;

private:
  /* 上方向 */
  glm::vec3 up;
  /* 观察方向起点 */
  glm::vec3 lookFromPoint;
  /* 观察方向终点 */
  glm::vec3 lookToPoint;
};


#endif // !SRC_CAMERA_CAMERA_H_
