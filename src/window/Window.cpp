#include "Window.h"
#include <iostream>

Window::Window(std::string title, int width, int height, bool isFullScreen, int pos_x = 0, int pos_y = 0) {
  // 初始化glfw
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "glfw library 初始化失败！" << std::endl;
    exit(EXIT_FAILURE);
  }

  GLFWmonitor* montitor = nullptr;

  if (isFullScreen) {
    montitor = glfwGetPrimaryMonitor();
  }

  glfwWindowHint(GLFW_POSITION_X, pos_x);
  glfwWindowHint(GLFW_POSITION_Y, pos_y);

  window = glfwCreateWindow(width, height, title.c_str(), montitor, nullptr);
  glfwMakeContextCurrent(window);

  // 初始化gl3w
  if (gl3wInit()) {
    std::cout << "OpenGL 初始化失败！" << std::endl;
    exit(EXIT_FAILURE);
  }
  // TODO... 其它窗口设置
}

Window::~Window() {
  glfwDestroyWindow(window);
}

bool Window::isClosed() {
  bool res = glfwWindowShouldClose(window);
  if (res == GLFW_FALSE) {
    return false;
  }
  return true;
}

void Window::swapBuffer() {
  glfwSwapBuffers(window);
}

void Window::pollEvents() {
  glfwPollEvents();
}
