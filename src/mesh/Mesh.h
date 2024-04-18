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
  /* 顶点位置 */
  std::vector<Vertex> vertices;
  /* 顶点索引 */
  std::vector<GLuint> indices;
  /* 材质*/
  Material material;

protected:
  void setUp();

protected:
  void createAndPopulateBuffer(GLuint& vao, GLuint& vbo, GLuint& ibo, std::vector<Vertex> _vertices, std::vector<GLuint> _indices);

  /* 顶点数组对象 */
  GLuint vao = 0;
  /* 顶点缓存对象 */
  GLuint vbo = 0;
  /* 索引缓存对象 */
  GLuint ibo = 0;
};

#endif // !SRC_MESH_MESH_H_
