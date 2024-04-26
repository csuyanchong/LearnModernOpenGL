#include "ProjectRenderToTexture.h"

#include <iostream>

ProjectRenderToTexture::ProjectRenderToTexture() {
  // 创建窗口
  createWindow();
  // 创建场景
  createScene();
  // 创建渲染管线
  createRenderPipeline();
  // 绑定输入
  bindInput();
}

ProjectRenderToTexture::~ProjectRenderToTexture() {
  cleanUp();
}

void ProjectRenderToTexture::run() {
  mainLoop();
}

void ProjectRenderToTexture::createWindow() {
  // 初始化glfw
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "glfw library 初始化失败！" << std::endl;
    exit(EXIT_FAILURE);
  }
  // 设置MSAA
  //glfwWindowHint(GLFW_SAMPLES, 4);
  // 初始化窗口位置
  glfwWindowHint(GLFW_POSITION_X, WINDOW_POSITION_X);
  glfwWindowHint(GLFW_POSITION_Y, WINDOW_POSITION_Y);
  // 创建窗口
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnMordenOpenGL", NULL, NULL);
  //glfwSetWindowPos(window, SCREEN_POSITION_X, SCREEN_POSITION_Y);
  glfwMakeContextCurrent(window);
  // 初始化gl3w
  gl3wInit();
}

void ProjectRenderToTexture::createScene() {
  // 平面
  bool resLoadModel = plane.loadFromFile(pathPlane);
  if (!resLoadModel) {
    std::cout << "从路径" + pathPlane + "加载模型失败！";
    exit(EXIT_FAILURE);
  }
  // 相机
  camMain.setEyePosition(glm::vec3(1, 2, 3));
  camMain.setLookDirection(glm::vec3(0, 0, -1));
  camMain.setUpDirection(glm::vec3(0, 1.0f, 0));
}

void ProjectRenderToTexture::createRenderPipeline() {
}

void ProjectRenderToTexture::bindInput() {
}

void ProjectRenderToTexture::mainLoop() {
  while (!glfwWindowShouldClose(window)) {
    draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void ProjectRenderToTexture::draw() {
  drawFirstPass();
  drawSecondPass();
}

void ProjectRenderToTexture::drawFirstPass() {
  // 渲染茶壶
}

void ProjectRenderToTexture::drawSecondPass() {
  // 渲染平面

}

void ProjectRenderToTexture::cleanUp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
