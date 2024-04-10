#ifndef SRC_MESH_MESH_H_
#define SRC_MESH_MESH_H_

#include <vector>

#include <gl3w/GL/gl3w.h>
#include <glm/vec3.hpp>


struct Vertex {
  glm::vec3 position;
};

class Mesh {
public:
  Mesh(std::vector<Vertex> _verts, std::vector<GLuint> _indices):vertices(_verts), indices(_indices) {};
  ~Mesh();
  void preDraw();
  void draw();

public:
  /* 顶点位置 */
  const std::vector<Vertex> vertices;
  /* 顶点索引 */
  const std::vector<GLuint> indices;
};

#endif // !SRC_MESH_MESH_H_
