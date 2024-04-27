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
  void draw();
  std::vector<Vertex> getVertices() const;

private:
  void setUp();
  void CreateAndPopulateBuffer();

private:
  /* ����λ�� */
  std::vector<Vertex> vertices;
  /* �������� */
  std::vector<GLuint> indices;

  /* ����������� */
  GLuint vao = 0;
  /* ���㻺����� */
  GLuint vbo = 0;
  /* ����������� */
  GLuint ibo = 0;
  /* λ���������� */
  GLuint positionAttrId = 0;
  /* ������������ */
  GLuint normalAttrId = 1;
  /* uv������������ */
  GLuint texcoordAttrId = 2;
};

#endif // !SRC_MESH_MESH_H_
