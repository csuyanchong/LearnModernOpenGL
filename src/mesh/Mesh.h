#ifndef SRC_MESH_MESH_H_
#define SRC_MESH_MESH_H_

#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <gl3w/GL/gl3w.h>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texcoord;
};

class Mesh {
public:
  Mesh(std::vector<Vertex> _verts, std::vector<GLuint> _indices);
  ~Mesh();
  void preDraw();
  void draw();

private:
  void setUp();
  /* 顶点位置 */
  std::vector<Vertex> vertices;
  /* 顶点索引 */
  std::vector<GLuint> indices;
};

#endif // !SRC_MESH_MESH_H_
