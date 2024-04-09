#ifndef SRC_MESH_BASICMESH_H_
#define SRC_MESH_BASICMESH_H_

#include <string>

class BasicMesh {
public:
  bool loadMeshFromFile(const std::string &path);
};

#endif // !SRC_MESH_BASICMESH_H_
