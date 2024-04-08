// Standard library header
#include <iostream>
#include <string>
#include <vector>

// Third-party library
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>

// Program src header
#include "shaderutil/ShaderUtil.h"

/* 屏幕宽度 */
static const int SCREEN_WIDTH = 1024;
/* 屏幕高度 */
static const int SCREEN_HEIGHT = 768;
/* 屏幕清除颜色 */
static const GLfloat CLEAR_COLOR[] = { 0.0f, 0.0f, 0.0f, 0.0f };
/* 窗口 */
GLFWwindow* window;

/* 顶点数组对象 */
GLuint vao = 0;
/* 顶点缓存对象 */
std::vector<GLuint> vbo(3);

/* 顶点位置 */
std::vector<GLfloat> verticesPosition;
/* 顶点颜色 */
std::vector<GLubyte> verticesColor;
/* 顶点索引 */
std::vector<GLuint> verticesIndex;

void preDraw() {
  glClearBufferfv(GL_COLOR, 0, CLEAR_COLOR);
}

void draw() {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glDrawElements(GL_TRIANGLES, verticesIndex.size(), GL_UNSIGNED_INT, NULL);
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
  // 初始化glfw
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "glfw library 初始化失败！" << std::endl;
    exit(EXIT_FAILURE);
  }
  // 创建窗口
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnMordenOpenGL", NULL, NULL);
  glfwMakeContextCurrent(window);
  // 初始化gl3w
  gl3wInit();
}

void createVertexData() {
  // 创建vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // 指定顶点数据
  verticesPosition = { 
    -0.5f, -0.5f, // 左下角
    0.5f, -0.5f, // 右下角
    0, 0.5f, // 上方
  };

  verticesColor = {
    255, 0, 0, // 红色
    0, 255, 0, // 绿色
    0, 0, 255 // 蓝色
  };

  verticesIndex = {
    0, 1, 2
  };

  // 顶点位置缓冲
  glGenBuffers(vbo.size(), vbo.data());
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, verticesPosition.size() * sizeof(GLfloat), verticesPosition.data(), GL_STATIC_DRAW);

  // 向vao解释attribute对应的缓存内容
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glDisableVertexAttribArray(0);

  // 顶点颜色缓冲
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesColor.size() * sizeof(GLubyte), verticesColor.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
  glDisableVertexAttribArray(1);

  // 索引缓冲
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesIndex.size() * sizeof(GLuint), verticesIndex.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(2, 3, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
  glDisableVertexAttribArray(2);
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
  // 1.初始化设置
  initSetup();
  // 2.创建顶点数据
  createVertexData();
  // 3.创建图形管线
  createGraphicPipeline();
  // 4.主循环
  mainLoop();
  // 5.清理
  cleanUp();
  return 0;
}