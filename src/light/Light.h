#ifndef SRC_LIGHT_LIGHT_H_
#define SRC_LIGHT_LIGHT_H_

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "../transform/Transform.h"

class Light {
public:
  Transform transform;
};

class DirectionalLight : public Light {
public:
  DirectionalLight() : from(glm::vec3(0)), to(glm::vec3(0, 0, -1)) {};

  glm::vec3 getDirection() const;

private:
  glm::vec3 from;
  glm::vec3 to;
};

class SpotLight : public Light {
public:
  SpotLight();
  SpotLight(glm::vec3 _pos, glm::vec3 _lookPoint, float _angle) : 
    position(_pos), 
    lookPoint(_lookPoint),
    angle(_angle) {};

  glm::mat4 getMatrix() const;
  glm::vec3 getLookDir() const;

  /* 光源位置 */
  glm::vec3 position;
  /* 光源朝向的点 */
  glm::vec3 lookPoint;
  /* 张开角度 */
  float angle;
};

#endif // !SRC_LIGHT_LIGHT_H_
