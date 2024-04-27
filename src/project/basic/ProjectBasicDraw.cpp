#include "ProjectBasicDraw.h"

// Standard library header
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// Third-party library
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <lodepng/lodepng.h>


// Program src header
#include "../../shaderutil/ShaderUtil.h"
#include "../../shaderutil/ShaderProgramUtil.h"
//#include "camera/Camera.h"
//#include "mesh/Mesh.h"
//#include "model/Model.h"
//#include "shaderutil/ShaderProgramUtil.h"
//#include "project/01_basic/ProjectBasicDraw.h"

ProjectBasicDraw::ProjectBasicDraw() {
  // 1.初始化设置
  initSetup();
  // 2.创建顶点数据
  createVertexData();
  // 3.创建图形管线
  createGraphicPipeline();
  setCamera();
}

ProjectBasicDraw::~ProjectBasicDraw() {
  // 5.清理
  cleanUp();
}

void ProjectBasicDraw::run() {
  // 4.主循环
  mainLoop();
}

void ProjectBasicDraw::initSetup() {
  // 初始化glfw
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "glfw library 初始化失败！" << std::endl;
    exit(EXIT_FAILURE);
  }
  // 设置MSAA
  glfwWindowHint(GLFW_SAMPLES, 4);
  // 创建窗口
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnMordenOpenGL", NULL, NULL);
  glfwSetWindowPos(window, SCREEN_POSITION_X, SCREEN_POSITION_Y);
  glfwMakeContextCurrent(window);
  // 初始化gl3w
  gl3wInit();
}

void ProjectBasicDraw::createVertexData() {
  // 加载模型文件
  bool resLoadModel = model.loadFromFile(pathModel);
  if (!resLoadModel) {
    std::cout << "从路径" + pathModel + "加载模型失败！";
    exit(EXIT_FAILURE);
  }
}

void ProjectBasicDraw::createGraphicPipeline() {
  std::vector<ShaderInfo> shaders;
  ShaderInfo vertInfo = {
    GL_VERTEX_SHADER, curVertShader
  };

  ShaderInfo fragInfo = {
    GL_FRAGMENT_SHADER, curFragShader
  };

  shaders.push_back(vertInfo);
  shaders.push_back(fragInfo);

  programPipeline = loadShader(shaders);
}

void ProjectBasicDraw::setCamera() {
  camMain.setEyePosition(glm::vec3(1, 2, 3));
  camMain.setLookDirection(glm::vec3(0, 0, -1));
  camMain.setUpDirection(glm::vec3(0, 1.0f, 0));
}

void ProjectBasicDraw::mainLoop() {
  while (!glfwWindowShouldClose(window))
  {
    preDraw();
    draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void ProjectBasicDraw::cleanUp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void ProjectBasicDraw::preDraw() {
  setClearFlag();
  // 使用图形管线
  glUseProgram(programPipeline);

  // 预计算着色参数
  preCompute();

  // 查询并修改全局变量
  ShaderProgramUtil programUtil(programPipeline);
  //passDataToShader(programUtil);
  //uniformBlinnPhongShader(programUtil);
  //uniformTextureMapShader(programUtil);
}

void ProjectBasicDraw::draw() {
}

void ProjectBasicDraw::setClearFlag() {
  // 清除颜色和深度缓冲
  glClearBufferfv(GL_COLOR, 0, CLEAR_COLOR);
  glClearBufferfv(GL_DEPTH, 0, &CLEAR_DEPTH);

  // 开启背面剔除
  //glEnable(GL_CULL_FACE);
  //glFrontFace(GL_CCW);
  //glCullFace(GL_BACK);

  // 隐藏面消除
  glEnable(GL_DEPTH_TEST);
  //// 开启MSAA
  //glEnable(GL_MULTISAMPLE);
}

void ProjectBasicDraw::preCompute() {
  // 计算mv, mvp, mvNormal
  //glm::mat4 modelMatrix = glm::mat4(1.0f);

  //modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, forward));

  //modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1.0f, 0));

  //modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

  //glm::mat4 viewMatrix = camMain.getViewMatrix();

  //glm::mat4 projectMatrix = glm::perspective(FOV, (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

  //modelView = viewMatrix * modelMatrix;
  //modelViewProjection = projectMatrix * viewMatrix * modelMatrix;

  //// 法线在view空间的变换
  //glm::mat4x4 modelViewForNormal44 = glm::transpose(glm::inverse(modelView));
  //modelViewForNormal = modelViewForNormal44;

  //// 计算dirLight。光线方向变换到view空间，不考虑平移，所以3*3矩阵就够了。
  //glm::mat3 modelLight = glm::rotate(glm::mat4(1.0f), glm::radians(lightRotationSpeed), glm::vec3(1.0f, 0, -1.0f));
  //glm::mat3 viewMatrix3 = viewMatrix;

  //glm::vec3 dirLightCompute = viewMatrix3 * modelLight * light.lightPosition;
  //dirLight = glm::normalize(dirLightCompute);
}

//void ProjectBasicDraw::passDataToShader(ShaderProgramUtil shaderUtil) {
//}
