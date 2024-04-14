#include "Mesh.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

namespace {
  /* ����������� */
  GLuint vao = 0;
  /* ���㻺����� */
  GLuint buffer_position_id = 0;
  GLuint buffer_normal_id = 1;
  GLuint buffer_texcoord_id = 2;
  GLuint buffer_index_id = 3;

  void CreateAndPopulateBuffer(std::vector<Vertex> _vertices, std::vector<GLuint> _indices) {
    // ����vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // ƫ������
    GLsizei strideVertex = 8 * sizeof(GLfloat);
    GLintptr vertex_position_offset = 0 * sizeof(GLfloat);
    GLintptr vertex_normal_offset = 3 * sizeof(GLfloat);
    GLintptr vertex_texcoord_offset = 6 * sizeof(GLfloat);

    // ����λ�û���
    glGenBuffers(1, &buffer_position_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_position_id);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

    // ��vao����attribute��Ӧ�Ļ�������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_position_offset);
    glDisableVertexAttribArray(0);

    // ���㷨�߻���
    glGenBuffers(1, &buffer_normal_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_normal_id);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_normal_offset);
    glDisableVertexAttribArray(1);

    // uv���껺��
    glGenBuffers(1, &buffer_texcoord_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_texcoord_id);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, strideVertex, (GLvoid*)vertex_texcoord_offset);
    glDisableVertexAttribArray(2);

    // ��������
    glGenBuffers(1, &buffer_index_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_index_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);
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
