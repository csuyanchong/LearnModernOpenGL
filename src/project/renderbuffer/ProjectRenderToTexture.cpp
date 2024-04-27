#include "ProjectRenderToTexture.h"

// stl
#include <iostream>

// plugin
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>

// program
#include "../../shaderutil/ShaderUtil.h"
#include "../../shaderutil/ShaderProgramUtil.h"

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
  std::vector<ShaderInfo> shaders;
  ShaderInfo vertInfo = {
    GL_VERTEX_SHADER, pathVertShader
  };

  ShaderInfo fragInfo = {
    GL_FRAGMENT_SHADER, pathFragShader
  };

  shaders.push_back(vertInfo);
  shaders.push_back(fragInfo);

  shaderProgram = loadShader(shaders);
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
  setClearBuffer();
  // ʹ��ͼ�ι���
  glUseProgram(shaderProgram);
  // ����shader�������ֵ
  computeShaderData();
  // �޸�shader����
  passDataToShader(shaderProgram);
  // ����ƽ��
  plane.draw();
}

void ProjectRenderToTexture::setClearBuffer() {
  glClearBufferfv(GL_COLOR, 0, CLEAR_COLOR);
  glClearBufferfv(GL_DEPTH, 0, &CLEAR_DEPTH);
  // ����������
  glEnable(GL_DEPTH_TEST);
}

void ProjectRenderToTexture::computeShaderData() {
  // ����mv, mvp, mvNormal
  glm::mat4 modelMatrix = glm::mat4(1.0f);

  modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, forward));

  modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1.0f, 0));

  modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

  glm::mat4 viewMatrix = camMain.getViewMatrix();

  glm::mat4 projectMatrix = glm::perspective(FOV, (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

  modelView = viewMatrix * modelMatrix;
  modelViewProjection = projectMatrix * viewMatrix * modelMatrix;

  // ������view�ռ�ı任
  glm::mat4x4 modelViewForNormal44 = glm::transpose(glm::inverse(modelView));
  modelViewForNormal = modelViewForNormal44;

  // ����dirLight�����߷���任��view�ռ䣬������ƽ�ƣ�����3*3����͹��ˡ�
  glm::mat3 modelLight = glm::rotate(glm::mat4(1.0f), glm::radians(lightRotationSpeed), glm::vec3(1.0f, 0, -1.0f));
  glm::mat3 viewMatrix3 = viewMatrix;

  glm::vec3 dirLightCompute = viewMatrix3 * modelLight * light.lightPosition;
  dirLight = glm::normalize(dirLightCompute);
}

void ProjectRenderToTexture::passDataToShader(GLuint shaderProgram) {
  // ��ѯ���޸�ȫ�ֱ���
  ShaderProgramUtil programUtil(shaderProgram);

  bool resModifyMVP = programUtil.glModifyUniformMat44("v_u_mvp", modelViewProjection);
  if (!resModifyMVP) {
    //exit(EXIT_FAILURE);
  }

  bool resModifySample2d_0 = programUtil.glModifyUniformInt1("f_u_sample2d_diffuse", 0);
  if (!resModifySample2d_0) {
    //exit(EXIT_FAILURE);
  }

  bool resModifySample2d_1 = programUtil.glModifyUniformInt1("f_u_sample2d_specular", 1);
  if (!resModifySample2d_1) {
    //exit(EXIT_FAILURE);
  }
}

void ProjectRenderToTexture::cleanUp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
