#ifndef SRC_MESH_BASICMESH_H_
#define SRC_MESH_BASICMESH_H_

#include <string>
#include <vector>
#include <glm/vec3.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
  glm::vec3 position;
};

class BasicMesh {
public:
  /* ����λ�� */
  const std::vector<Vertex> vertices;
  /* �������� */
  const std::vector<unsigned int> indices;
  bool loadMeshFromFile(const std::string &path);
private:
  void processNode(aiNode* node, const aiScene* scene);
};

#endif // !SRC_MESH_BASICMESH_H_
