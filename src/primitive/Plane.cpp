#include "Plane.h"
#include <string>

namespace {
  std::vector<Vertex> createVertices() {
    std::vector<Vertex> res;

    Vertex vert0 = {
      glm::vec3(-1.0f, 0, 1.0f),
      glm::vec3(0, 1.0f, 0),
      glm::vec2(0, 1.0f)
    };

    Vertex vert1 = {
      glm::vec3(-1.0f, 0, -1.0f),
      glm::vec3(0, 1.0f, 0),
      glm::vec2(0, 0)
    };

    Vertex vert2 = {
      glm::vec3(1.0f, 0, -1.0f),
      glm::vec3(0, 1.0f, 0),
      glm::vec2(1.0f, 0)
    };

    Vertex vert3 = {
      glm::vec3(1.0f, 0, 1.0f),
      glm::vec3(0, 1.0f, 0),
      glm::vec2(1.0f, 1.0f) 
    };

    res.push_back(vert0);
    res.push_back(vert1);
    res.push_back(vert2);
    res.push_back(vert3);
    return res;
  }

  std::vector<GLuint> createIntices() {
    std::vector<GLuint> res = { 0, 1, 2, 0, 2, 3};
    return res;
  }
}

Plane::Plane() {
  /* 顶点位置 */
  std::vector<Vertex> vertices = createVertices();
  /* 顶点索引 */
  std::vector<GLuint> indices = createIntices();
  Mesh mesh(vertices, indices);
  Material mat;
  mesh.SetMaterial(mat);
  meshes.push_back(mesh);
}

void Plane::preDraw() {
}

void Plane::draw() {

}
