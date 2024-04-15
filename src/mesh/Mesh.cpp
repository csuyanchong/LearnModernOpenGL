#include "Mesh.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

namespace {
  /* 顶点数组对象 */
  GLuint vao = 0;
  /* 顶点缓存对象 */
  GLuint vbo = 0;
  /* 索引缓存对象 */
  GLuint ibo = 0;

  void CreateAndPopulateBuffer(std::vector<Vertex> _vertices, std::vector<GLuint> _indices) {
    // 创建vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 创建索引缓存
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);

    // 创建顶点缓存
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

    // 向vao解释attribute对应的缓存内容
    
    // 偏移参数设置
    GLsizei strideVertex = sizeof(Vertex);
    GLintptr vertex_position_offset = 0 * sizeof(GLfloat);
    GLintptr vertex_normal_offset = 3 * sizeof(GLfloat);
    GLintptr vertex_texcoord_offset = 6 * sizeof(GLfloat);
    
    // 顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_position_offset);

    // 顶点法线属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_normal_offset);

    // uv坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_texcoord_offset);

    // 创建完先关闭，绘制前再启用
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }
}

Mesh::Mesh(std::vector<Vertex> _verts, std::vector<GLuint> _indices):vertices(_verts), indices(_indices) {
  setUp();
}

Mesh::~Mesh() {
}

void Mesh::preDraw() {
  // 绑定顶点缓存
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
}

void Mesh::draw() {
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, NULL);
}

void Mesh::setUp() {
  // 创建OpenGL缓存
  CreateAndPopulateBuffer(vertices, indices);
}
