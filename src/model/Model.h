#ifndef SRC_MODEL_MODEL_H_
#define SRC_MODEL_MODEL_H_

/* Standard library */
#include <string>
#include <vector>

/* Src header */
#include "../sceneobject/SceneObject.h"
#include "../mesh/Mesh.h"
#include "../texture/Texture.h"
#include "../texture/GLTextureManager.h"

class Model : public SceneObject{
public:
  Model();
  ~Model();
  bool loadFromFile(const std::string& filePath);
  void preDraw();
  void draw();

public:
  std::vector<Mesh> meshes;
  std::vector<Texture> textures;
  GLTextureManager textureManager;
};

#endif // !SRC_MODEL_MODEL_H_



