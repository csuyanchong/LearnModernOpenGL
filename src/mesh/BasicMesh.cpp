#include <iostream>
#include "BasicMesh.h"
#include <gl3w/GL/gl3w.h>

bool BasicMesh::loadMeshFromFile(const std::string& path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
  if (nullptr == scene) {
    std::cout << importer.GetErrorString() << std::endl;
    return false;
  }

  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

  processNode(scene->mRootNode, scene);
  return true;
}

void BasicMesh::processNode(aiNode* node, const aiScene* scene) {
  for (GLuint i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
  /*  meshes_.push_back(this->processMesh(mesh, scene));*/
  }

  for (GLuint i = 0; i < node->mNumChildren; i++) {
    this->processNode(node->mChildren[i], scene);
  }
}
