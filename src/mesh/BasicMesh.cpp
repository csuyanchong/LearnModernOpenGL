#include <iostream>
#include "BasicMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

bool BasicMesh::loadMeshFromFile(const std::string& path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
  if (nullptr == scene) {
    std::cout << importer.GetErrorString() << std::endl;
    return false;
  }

  // TODO...处理模型数据。
  return true;
}
