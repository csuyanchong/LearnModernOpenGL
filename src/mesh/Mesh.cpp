#include "Mesh.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

namespace {
  /* ����������� */
  GLuint vao = 0;
  /* ���㻺����� */
  std::vector<GLuint> vbo(2);

  void CreateAndPopulateBuffer(std::vector<Vertex> _vertices, std::vector<GLuint> _indices) {
    // ����vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // ����λ�û���
    glGenBuffers((GLsizei)vbo.size(), vbo.data());
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

    // ��vao����attribute��Ӧ�Ļ�������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDisableVertexAttribArray(0);

    // ��������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
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
  //glEnableVertexAttribArray(1);

}

void Mesh::draw() {
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, NULL);
}

void Mesh::setUp() {
  // ����OpenGL����
  CreateAndPopulateBuffer(vertices, indices);
}
