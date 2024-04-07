// Standard library header
#include <iostream>
#include <string>
#include <vector>

// Third-party library
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>

// Prgram src header
#include "shaderutil/ShaderUtil.h"

/* 屏幕宽度 */
static const int SCREEN_WIDTH = 1024;
/* 屏幕高度 */
static const int SCREEN_HEIGHT = 768;
/* 屏幕清除颜色 */
static const GLfloat CLEAR_COLOR[] = { 0.0f, 0.0f, 0.0f, 0.0f };
/* 窗口 */
GLFWwindow* window;

// 顶点数组对象
GLuint vao = 0;

// 顶点缓存对象
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
  std::vector<GLfloat> vertices = { 
    -0.5f, -0.5f, // 左下角
    0.5f, -0.5f, // 右下角
    0, 0.5f // 上方
  };

  // 创建vbo
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

  // 向vao解释attribute对应的缓存内容
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