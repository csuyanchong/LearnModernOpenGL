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
  // �󶨶��㻺��
  glBindVertexArray(vao);
  glEnableVertexAttribArray(positionAttrId);
  glEnableVertexAttribArray(normalAttrId);
  glEnableVertexAttribArray(texcoordAttrId);

  // ����
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);
}

std::vector<Vertex> Mesh::getVertices() const {
  return vertices;
}

void Mesh::setUp() {
  // ����OpenGL����
  CreateAndPopulateBuffer();
}

void Mesh::CreateAndPopulateBuffer() {
  // ����vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // ������������
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  // �������㻺��
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  // ��vao����attribute��Ӧ�Ļ�������

  // ƫ�Ʋ�������
  GLsizei strideVertex = sizeof(Vertex);
  GLintptr vertex_position_offset = 0 * sizeof(GLfloat);
  GLintptr vertex_normal_offset = 3 * sizeof(GLfloat);
  GLintptr vertex_texcoord_offset = 6 * sizeof(GLfloat);

  // ����λ������
  glVertexAttribPointer(positionAttrId, 3, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_position_offset);

  // ���㷨������
  glVertexAttribPointer(normalAttrId, 3, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_normal_offset);

  // uv��������
  glVertexAttribPointer(texcoordAttrId, 2, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_texcoord_offset);

  // �������ȹرգ�����ǰ������
  glDisableVertexAttribArray(positionAttrId);
  glDisableVertexAttribArray(normalAttrId);
  glDisableVertexAttribArray(texcoordAttrId);
}