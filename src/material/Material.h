#ifndef SRC_MATERIAL_MATERIAL_H_
#define SRC_MATERIAL_MATERIAL_H_

#include "../shader/Shader.h"

class Material {
public:
  Material();
  Material(Shader& _shader);
  ~Material();

  Shader* shader;
};


#endif // !SRC_MATERIAL_MATERIAL_H_


