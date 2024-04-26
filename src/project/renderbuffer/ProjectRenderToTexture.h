#ifndef SRC_PROJECT_RENDERBUFFER_PROJECTRENDERTOTEXTURE_H_
#define SRC_PROJECT_RENDERBUFFER_PROJECTRENDERTOTEXTURE_H_

// Standard library
#include <string>

// Third party
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>

// Program defined
#include "../../model/Model.h"
#include "../../camera/Camera.h"


class ProjectRenderToTexture {
public:
  ProjectRenderToTexture();
  ~ProjectRenderToTexture();
  void run();

private:
  void createWindow();
  void createScene();
  void createRenderPipeline();
  void bindInput();
  void mainLoop();
  void draw();
  void drawFirstPass();
  void drawSecondPass();
  void cleanUp();
private:
  /* 窗口 */
  GLFWwindow* window;
  /* 屏幕宽度 */
  int SCREEN_WIDTH = 1280;
  /* 屏幕高度 */
  int SCREEN_HEIGHT = 720;
  int WINDOW_POSITION_X = 640;
  int WINDOW_POSITION_Y = 200;
  /* 屏幕清除颜色 */
  GLfloat CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  /* 屏幕深度缓存清除值 */
  GLfloat CLEAR_DEPTH = 1.0f;

  Model plane;
  /* 模型文件地址 */
  const std::string MODELS_DIR = "./data/models/objmodel/";
  std::string namePlane = "plane.obj";
  std::string pathPlane = MODELS_DIR + namePlane;

  /* 相机 */
  Camera camMain;
};

#endif // !SRC_PROJECT_RENDERBUFFER_PROJECTRENDERTOTEXTURE_H_

