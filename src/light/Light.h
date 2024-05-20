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
  SpotLight() : angle(60), lookFromPoint(glm::vec3(0)), lookToPoint(glm::vec3(0, 0, -1.0f)) {};
  SpotLight(float _angle) : angle(_angle), lookFromPoint(glm::vec3(0)), lookToPoint(glm::vec3(0, 0, -1.0f)) {};

  glm::mat4 getMatrix() const;
  glm::vec3 getLookDir() const;

public:
  /* �ſ��Ƕ� */
  float angle;

private:
  /* �۲췽����� */
  glm::vec3 lookFromPoint;
  /* �۲췽���յ� */
  glm::vec3 lookToPoint;
};

#endif // !SRC_LIGHT_LIGHT_H_
