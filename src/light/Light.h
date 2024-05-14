#ifndef SRC_LIGHT_LIGHT_H_
#define SRC_LIGHT_LIGHT_H_

#include <glm/vec3.hpp>


class SpotLight {
public:
  SpotLight();
  SpotLight(glm::vec3 _pos, glm::vec3 _toward, float _angle) : 
    position(_pos), 
    toward(_toward), 
    angle(_angle) {};

  /* 光源位置 */
  glm::vec3 position;
  /* 光源朝向 */
  glm::vec3 toward;
  /* 张开角度 */
  float angle;
};

#endif // !SRC_LIGHT_LIGHT_H_
