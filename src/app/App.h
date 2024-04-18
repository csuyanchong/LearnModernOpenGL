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
  /* ���� */
  Window* window;
  /* ���� */
  Scene* scene;
  /* ��Ⱦͨ�� */
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



