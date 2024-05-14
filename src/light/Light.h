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

  /* ��Դλ�� */
  glm::vec3 position;
  /* ��Դ���� */
  glm::vec3 toward;
  /* �ſ��Ƕ� */
  float angle;
};

#endif // !SRC_LIGHT_LIGHT_H_
