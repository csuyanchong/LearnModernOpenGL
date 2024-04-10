#ifndef SRC_MODEL_MODEL_H_
#define SRC_MODEL_MODEL_H_

/* Standard library */
#include <string>
#include <vector>

/* Src header */
#include "../mesh/Mesh.h"

class Model {
public:
  Model();
  ~Model();
  bool loadFromFile(const std::string& filePath);
  void preDraw();
  void draw();

private:
  std::vector<Mesh> meshes;
};

#endif // !SRC_MODEL_MODEL_H_



