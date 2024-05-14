#ifndef SRC_TRANSFORM_TRANSFORM_H_
#define SRC_TRANSFORM_TRANSFORM_H_

#include <glm/vec3.hpp>

class Transform
{
public:
  Transform();
  Transform(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale) :
    position(_pos), rotation(_rot), scale(_scale) {};

  glm::vec3 position;
  /* 先用欧拉角表示，TODO...四元数 */
  glm::vec3 rotation;
  glm::vec3 scale;
};
#endif // !SRC_TRANSFORM_TRANSFORM_H_