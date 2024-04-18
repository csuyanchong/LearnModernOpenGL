#ifndef SRC_APP_APP_H_
#define SRC_APP_APP_H_

#include "../window/Window.h"
#include "../scene/Scene.h"
#include "../render/RenderPass.h"

class App {
public:
  App();
  ~App();
  void initApp();
  void runApp();
  void exitApp();

public:
  /* 窗口 */
  Window* window;
  /* 场景 */
  Scene* scene;
  /* 渲染通道 */
  std::vector<RenderPass>  renderPassList;

private:
  void initLibray();
  void setupApp();
  void setupWindow();
  void setupRenderPass();
  void createScene();

  void mainLoop();
  void render();

  void cleanLibrary();
};



#endif // !SRC_APP_APP_H_



