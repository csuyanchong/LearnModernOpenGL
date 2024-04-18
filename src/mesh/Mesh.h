#ifndef SRC_MESH_MESH_H_
#define SRC_MESH_MESH_H_

#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <gl3w/GL/gl3w.h>

#include "../material/Material.h"

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
  void SetMaterial(Material _mat);

public:
  /* ����λ�� */
  std::vector<Vertex> vertices;
  /* �������� */
  std::vector<GLuint> indices;
  /* ����*/
  Material material;

protected:
  void setUp();

protected:
  void createAndPopulateBuffer(GLuint& vao, GLuint& vbo, GLuint& ibo, std::vector<Vertex> _vertices, std::vector<GLuint> _indices);

  /* ����������� */
  GLuint vao = 0;
  /* ���㻺����� */
  GLuint vbo = 0;
  /* ����������� */
  GLuint ibo = 0;
};

#endif // !SRC_MESH_MESH_H_
