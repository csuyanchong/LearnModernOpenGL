#ifndef SRC_WINDOW_WINDOW_H_
#define SRC_WINDOW_WINDOW_H_

#include <string>
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>

class Window {
public:
  Window(std::string title, int width, int height, bool isFullScreen, int pos_x, int pos_y);

  ~Window();

  bool isClosed();
  void swapBuffer();
  void pollEvents();

public:
  GLFWwindow* window;
};

#endif // !SRC_WINDOW_WINDOW_H_


