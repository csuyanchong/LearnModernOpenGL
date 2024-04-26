#include "ProjectRenderToTexture.h"

#include <iostream>

ProjectRenderToTexture::ProjectRenderToTexture() {
  // ��������
  createWindow();
  // ��������
  createScene();
  // ������Ⱦ����
  createRenderPipeline();
  // ������
  bindInput();
}

ProjectRenderToTexture::~ProjectRenderToTexture() {
  cleanUp();
}

void ProjectRenderToTexture::run() {
  mainLoop();
}

void ProjectRenderToTexture::createWindow() {
  // ��ʼ��glfw
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "glfw library ��ʼ��ʧ�ܣ�" << std::endl;
    exit(EXIT_FAILURE);
  }
  // ����MSAA
  //glfwWindowHint(GLFW_SAMPLES, 4);
  // ��ʼ������λ��
  glfwWindowHint(GLFW_POSITION_X, WINDOW_POSITION_X);
  glfwWindowHint(GLFW_POSITION_Y, WINDOW_POSITION_Y);
  // ��������
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnMordenOpenGL", NULL, NULL);
  //glfwSetWindowPos(window, SCREEN_POSITION_X, SCREEN_POSITION_Y);
  glfwMakeContextCurrent(window);
  // ��ʼ��gl3w
  gl3wInit();
}

void ProjectRenderToTexture::createScene() {
  // ƽ��
  bool resLoadModel = plane.loadFromFile(pathPlane);
  if (!resLoadModel) {
    std::cout << "��·��" + pathPlane + "����ģ��ʧ�ܣ�";
    exit(EXIT_FAILURE);
  }
  // ���
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
  // ��Ⱦ���
}

void ProjectRenderToTexture::drawSecondPass() {
  // ��Ⱦƽ��

}

void ProjectRenderToTexture::cleanUp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
