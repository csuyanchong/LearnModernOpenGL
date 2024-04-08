// Standard library header
#include <iostream>
#include <string>
#include <vector>

// Third-party library
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>

// Program src header
#include "shaderutil/ShaderUtil.h"

/* ��Ļ��� */
static const int SCREEN_WIDTH = 1024;
/* ��Ļ�߶� */
static const int SCREEN_HEIGHT = 768;
/* ��Ļ�����ɫ */
static const GLfloat CLEAR_COLOR[] = { 0.0f, 0.0f, 0.0f, 0.0f };
/* ���� */
GLFWwindow* window;

/* ����������� */
GLuint vao = 0;
/* ���㻺����� */
std::vector<GLuint> vbo(3);

/* ����λ�� */
std::vector<GLfloat> verticesPosition;
/* ������ɫ */
std::vector<GLubyte> verticesColor;
/* �������� */
std::vector<GLuint> verticesIndex;

/* ͼ�ι��� */
GLuint programPipeline;

GLfloat uOffsetX = 0;
GLfloat uOffsetY = 0;


void preDraw() {
  glClearBufferfv(GL_COLOR, 0, CLEAR_COLOR);

  // ʹ��ͼ�ι���
  glUseProgram(programPipeline);

  // ��ѯ���޸�ȫ�ֱ���
  GLint location_uniform_OffsetX = glGetUniformLocation(programPipeline, "uOffsetX");
  if (location_uniform_OffsetX != -1) {
    glUniform1f(location_uniform_OffsetX, uOffsetX);
  }
  else {
    std::cout << "��ѯȫ�ֱ���uOffsetXû�ҵ�������ƴд����" << std::endl;
  }

  GLint location_uniform_OffsetY = glGetUniformLocation(programPipeline, "uOffsetY");
  if (location_uniform_OffsetY != -1) {
    glUniform1f(location_uniform_OffsetY, uOffsetY);
  }
  else {
    std::cout << "��ѯȫ�ֱ���uOffsetYû�ҵ�������ƴд����" << std::endl;
  }
}

void draw() {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glDrawElements(GL_TRIANGLES, (GLsizei)verticesIndex.size(), GL_UNSIGNED_INT, NULL);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else if (key == GLFW_KEY_LEFT) {
    uOffsetX -= 0.01f;
  }
  else if (key == GLFW_KEY_RIGHT) {
    uOffsetX += 0.01f;
  }
  else if (key == GLFW_KEY_UP) {
    uOffsetY += 0.01f;
  }
  else if (key == GLFW_KEY_DOWN) {
    uOffsetY -= 0.01f;
  }
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void initSetup() {
  // ��ʼ��glfw
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "glfw library ��ʼ��ʧ�ܣ�" << std::endl;
    exit(EXIT_FAILURE);
  }
  // ��������
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnMordenOpenGL", NULL, NULL);
  glfwMakeContextCurrent(window);
  // ��ʼ��gl3w
  gl3wInit();
  // �����¼��ص�
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
}

void createVertexData() {
  // ����vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // ָ����������
  verticesPosition = {
    -0.5f, 0.5f,
    -0.5f, -0.5f,
    0.5f, -0.5f,
    0.5f, 0.5f
  };

  verticesColor = {
    255, 0, 0, // ��ɫ
    0, 255, 0, // ��ɫ
    0, 0, 255 // ��ɫ
  };

  verticesIndex = {
    0, 1, 2, 0, 2, 3
  };

  // ����λ�û���
  glGenBuffers((GLsizei)vbo.size(), vbo.data());
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, verticesPosition.size() * sizeof(GLfloat), verticesPosition.data(), GL_STATIC_DRAW);

  // ��vao����attribute��Ӧ�Ļ�������
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glDisableVertexAttribArray(0);

  // ������ɫ����
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesColor.size() * sizeof(GLubyte), verticesColor.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
  glDisableVertexAttribArray(1);

  // ��������
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesIndex.size() * sizeof(GLuint), verticesIndex.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(2, 3, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
  glDisableVertexAttribArray(2);
}

void createGraphicPipeline() {
  std::vector<ShaderInfo> shaders;
  ShaderInfo vertInfo = {
    GL_VERTEX_SHADER, "./data/shaders/vert.glsl"
  };

  ShaderInfo fragInfo = {
    GL_FRAGMENT_SHADER, "./data/shaders/frag.glsl" 
  };

  shaders.push_back(vertInfo);
  shaders.push_back(fragInfo);

  programPipeline = loadShader(shaders);
}

void mainLoop() {
  while (!glfwWindowShouldClose(window))
  {
    preDraw();
    draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void cleanUp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

int main(int argc, char* argv[]) {
  // 1.��ʼ������
  initSetup();
  // 2.������������
  createVertexData();
  // 3.����ͼ�ι���
  createGraphicPipeline();
  // 4.��ѭ��
  mainLoop();
  // 5.����
  cleanUp();
  return 0;
}