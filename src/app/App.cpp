#include "App.h"

// Standard library header
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// Third-party library
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <lodepng/lodepng.h>

#include "../scene/Scene.h"
#include "../render/RenderToTexturePass.h"
#include "../render/NormalRenderPass.h"

App::App() {
  window = nullptr;
  scene = nullptr;
}

App::~App() {
  delete window;
  delete scene;
}

void App::initApp() {
  initLibray();
  setupApp();
  createScene();
}

void App::runApp() {
  mainLoop();
}

void App::exitApp() {
  cleanLibrary();
}

void App::initLibray() {
 
}

void App::setupApp() {
  // 创建窗口
  setupWindow();
  // 设置渲染通道
  setupRenderPass();
}

void App::setupWindow() {
  // TODO...读取xml配置文件s
  window = new Window("LearnMordenOpenGL", 1280, 720, false, 200, 200);
}

void App::setupRenderPass() {
  RenderToTexturePass pass1;
  RenderToTexturePass pass2;
  renderPassList.push_back(pass1);
  renderPassList.push_back(pass2);
}

void App::createScene() {
  scene = new Scene();
}

void App::mainLoop() {
  while (!window->isClosed())
  {
    render();

    window->swapBuffer();

    window->pollEvents();
  }
}

void App::render() {
  for (size_t i = 0; i < renderPassList.size(); i++) {
    RenderPass renderPass = renderPassList[i];
    renderPass.render();
  }
}

void App::cleanLibrary() {
  glfwTerminate();
}
