#include "Mesh.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

Mesh::Mesh(std::vector<Vertex> _verts, std::vector<GLuint> _indices):vertices(_verts), indices(_indices) {
  setUp();
}

Mesh::~Mesh() {
}

void Mesh::draw() {
  // 绑定顶点缓存
  glBindVertexArray(vao);
  glEnableVertexAttribArray(positionAttrId);
  glEnableVertexAttribArray(normalAttrId);
  glEnableVertexAttribArray(texcoordAttrId);

  // 绘制
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);
}

std::vector<Vertex> Mesh::getVertices() const {
  return vertices;
}

void Mesh::setUp() {
  // 创建OpenGL缓存
  CreateAndPopulateBuffer();
}

void Mesh::CreateAndPopulateBuffer() {
  // 创建vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // 创建索引缓存
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  // 创建顶点缓存
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  // 向vao解释attribute对应的缓存内容

  // 偏移参数设置
  GLsizei strideVertex = sizeof(Vertex);
  GLintptr vertex_position_offset = 0 * sizeof(GLfloat);
  GLintptr vertex_normal_offset = 3 * sizeof(GLfloat);
  GLintptr vertex_texcoord_offset = 6 * sizeof(GLfloat);

  // 顶点位置属性
  glVertexAttribPointer(positionAttrId, 3, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_position_offset);

  // 顶点法线属性
  glVertexAttribPointer(normalAttrId, 3, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_normal_offset);

  // uv坐标属性
  glVertexAttribPointer(texcoordAttrId, 2, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_texcoord_offset);

  // 创建完先关闭，绘制前再启用
  glDisableVertexAttribArray(positionAttrId);
  glDisableVertexAttribArray(normalAttrId);
  glDisableVertexAttribArray(texcoordAttrId);
}