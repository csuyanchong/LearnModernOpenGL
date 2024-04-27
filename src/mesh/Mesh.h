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
  /* 顶点位置 */
  std::vector<Vertex> vertices;
  /* 顶点索引 */
  std::vector<GLuint> indices;

  /* 顶点数组对象 */
  GLuint vao = 0;
  /* 顶点缓存对象 */
  GLuint vbo = 0;
  /* 索引缓存对象 */
  GLuint ibo = 0;
  /* 位置属性索引 */
  GLuint positionAttrId = 0;
  /* 法线属性索引 */
  GLuint normalAttrId = 1;
  /* uv坐标属性索引 */
  GLuint texcoordAttrId = 2;
};

#endif // !SRC_MESH_MESH_H_
