// Standard library header
#include <iostream>
#include <string>
#include <vector>

// Third-party library
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>

// Prgram src header
#include "shaderutil/ShaderUtil.h"

/* ��Ļ��� */
static const int SCREEN_WIDTH = 1024;
/* ��Ļ�߶� */
static const int SCREEN_HEIGHT = 768;
/* ��Ļ�����ɫ */
static const GLfloat CLEAR_COLOR[] = { 0.0f, 0.0f, 0.0f, 0.0f };
/* ���� */
GLFWwindow* window;

// �����������
GLuint vao = 0;

// ���㻺�����
GLuint vbo = 0;

static const std::string VERTEX_SHADER_PATH = "";

void preDraw() {
  glClearBufferfv(GL_COLOR, 0, CLEAR_COLOR);
}

void Draw() {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
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
}

void createVertexData() {
  // ����vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // ָ����������
  std::vector<GLfloat> vertices = { 
    -0.5f, -0.5f, // ���½�
    0.5f, -0.5f, // ���½�
    0, 0.5f // �Ϸ�
  };

  // ����vbo
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

  // ��vao����attribute��Ӧ�Ļ�������
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glDisableVertexAttribArray(0);
}

void createGraphicPipeline() {
  GLuint program = 0;
  std::vector<ShaderInfo> shaders;
  ShaderInfo vertInfo = {
    GL_VERTEX_SHADER, "./data/shaders/vert.glsl"
  };

  ShaderInfo fragInfo = {
    GL_FRAGMENT_SHADER, "./data/shaders/frag.glsl" 
  };

  shaders.push_back(vertInfo);
  shaders.push_back(fragInfo);

  program = loadShader(shaders);
  glUseProgram(program);
}

void mainLoop() {
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

  while (!glfwWindowShouldClose(window))
  {
    preDraw();
    Draw();
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