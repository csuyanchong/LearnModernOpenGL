// Standard library header
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// Third-party library
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>

// Program src header
#include "shaderutil/ShaderUtil.h"
#include "camera/Camera.h"
#include "mesh/Mesh.h"
#include "model/Model.h"

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

/* 图形管线 */
GLuint programPipeline;

/* 透视投影 */
GLfloat FOV = 45.0f;
GLfloat NEAR_CLIP_PLANE = 0.1f;
GLfloat FAR_CLIP_PLANE = 100.0f;

/* 模型变换 */
GLfloat rotation = 0;
GLfloat forward = 0;
GLfloat scale = 0.01f;

/* 主摄像机 */
Camera camMain;
glm::vec3 camPos = glm::vec3(1, 2, 3);

GLfloat moveCamVerticalSpeed = 0.1f;
GLfloat moveCamHorizenSpeed = 0.1f;

/* 模型文件地址 */
const std::string MODELS_DIR = "./data/models/objmodel/";
std::string nameModel = "teapot.obj";

std::string pathModel = MODELS_DIR + nameModel;

Model model;

/* 模型材质颜色 */
glm::vec3 materialColor = glm::vec3(1.0f, 0, 0);

/* 平行光Dirction light光照模型 */
glm::vec3 lightDir = glm::vec3(-1, -3, -3);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

void preDraw() {
  glClearBufferfv(GL_COLOR, 0, CLEAR_COLOR);
  glEnable(GL_DEPTH);
  //glEnable(GL_STENCIL);

  // 使用图形管线
  glUseProgram(programPipeline);
  
  model.preDraw();

  // 设置MVP矩阵
  glm::mat4 modelMatrix = glm::mat4(1.0f);

  modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, forward));

  modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1.0f, 0));

  modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

  camMain.setEyePosition(camPos);
  glm::mat4 viewMatrix = camMain.getViewMatrix();

  glm::mat4 projectMatrix = glm::perspective(FOV, (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

  glm::mat4 mvp = projectMatrix * viewMatrix * modelMatrix;

  // 查询并修改全局变量
  GLint location_uniform_projMat = glGetUniformLocation(programPipeline, "uMVP");
  if (location_uniform_projMat != -1) {
    glUniformMatrix4fv(location_uniform_projMat, 1, GL_FALSE, &mvp[0][0]);
  }
  else {
    std::cout << "查询全局变量projMat没找到，可能拼写错误！" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void draw() {
  model.draw();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  if (key == GLFW_KEY_W) {
    camMain.moveForward(moveCamVerticalSpeed);
  }
  if (key == GLFW_KEY_S) {
    camMain.moveBack(moveCamVerticalSpeed);
  }

  if (key == GLFW_KEY_A) {
    camMain.moveLeft(moveCamHorizenSpeed);
  }
  if (key == GLFW_KEY_D) {
    camMain.moveRight(moveCamHorizenSpeed);
  }

  if (key == GLFW_KEY_LEFT) {
    rotation -= 5.0f;
  }
  if (key == GLFW_KEY_RIGHT) {
    rotation += 5.0f;
  }
  if (key == GLFW_KEY_UP) {
    forward += 0.1f;
  }
  if (key == GLFW_KEY_DOWN) {
    forward -= 0.1f;
  }
  if (key == GLFW_KEY_T) {
    scale += 0.1f;
    scale = fmin(scale, 2.0f);
  }
  if (key == GLFW_KEY_G) {
    scale -= 0.1f;
    scale = fmaxf(scale, 0.2f);
  }
  if (key == GLFW_KEY_N)
  {
    scale = 1;
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
  // 键盘事件回调
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
}

void createVertexData() {
  // 加载模型文件
  bool resLoadModel = model.loadFromFile(pathModel);
  if (!resLoadModel) {
    std::cout << "从路径" + pathModel + "加载模型失败！";
    exit(EXIT_FAILURE);
  }
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