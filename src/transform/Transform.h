#ifndef SRC_TRANSFORM_TRANSFORM_H_
#define SRC_TRANSFORM_TRANSFORM_H_

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Transform
{
public:
  Transform();
  Transform(glm::vec3 _pos, glm::vec3 _euler, glm::vec3 _scale) :
    position(_pos), euler(_euler), scale(_scale) {};

  Transform& operator=(const Transform& other);

  glm::mat4 getMatrix() const;

  glm::vec3 position;
  /* 先用欧拉角表示，TODO...四元数 */
  glm::vec3 euler;
  glm::vec3 scale;
};
#endif // !SRC_TRANSFORM_TRANSFORM_H_