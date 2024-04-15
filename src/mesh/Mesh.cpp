#include "Mesh.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

namespace {
  /* ����������� */
  GLuint vao = 0;
  /* ���㻺����� */
  GLuint vbo = 0;
  /* ����������� */
  GLuint ibo = 0;

  void CreateAndPopulateBuffer(std::vector<Vertex> _vertices, std::vector<GLuint> _indices) {
    // ����vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // ������������
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);

    // �������㻺��
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

    // ��vao����attribute��Ӧ�Ļ�������
    
    // ƫ�Ʋ�������
    GLsizei strideVertex = sizeof(Vertex);
    GLintptr vertex_position_offset = 0 * sizeof(GLfloat);
    GLintptr vertex_normal_offset = 3 * sizeof(GLfloat);
    GLintptr vertex_texcoord_offset = 6 * sizeof(GLfloat);
    
    // ����λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_position_offset);

    // ���㷨������
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_normal_offset);

    // uv��������
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_texcoord_offset);

    // �������ȹرգ�����ǰ������
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
  // �󶨶��㻺��
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
}

void Mesh::draw() {
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, NULL);
}

void Mesh::setUp() {
  // ����OpenGL����
  CreateAndPopulateBuffer(vertices, indices);
}
