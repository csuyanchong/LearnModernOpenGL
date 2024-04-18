#include "Window.h"

Window::Window(std::string title, int width, int height, bool isFullScreen, int pos_x = 0, int pos_y = 0) {
  GLFWmonitor* montitor = nullptr;

  if (isFullScreen) {
    montitor = glfwGetPrimaryMonitor();
  }

  window = glfwCreateWindow(width, height, title.c_str(), montitor, nullptr);
  glfwMakeContextCurrent(window);

  glfwWindowHint(GLFW_POSITION_X, pos_x);
  glfwWindowHint(GLFW_POSITION_Y, pos_y);
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
