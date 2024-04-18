#include "Material.h"
#include "../shader/BlinnPhongShader.h"

Material::Material() {
  //  π”√BlinnPhong shader
  shader = new BlinnPhongShader();
}

Material::~Material() {
}

Material::Material(Shader& _shader) {
  shader = &_shader;
}
