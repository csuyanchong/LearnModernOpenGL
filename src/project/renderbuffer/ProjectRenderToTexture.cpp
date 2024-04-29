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
  // 创建窗口
  createWindow();
  // 创建场景
  createScene();
  // 创建渲染管线
  createRenderPipeline();
  // 绑定输入
  bindInput();
  // 创建framebuffer
  createFrameBuffer();
}

ProjectRenderToTexture::~ProjectRenderToTexture() {
  cleanUp();
}

void ProjectRenderToTexture::run() {
  mainLoop();
}

void ProjectRenderToTexture::createWindow() {
  // 初始化glfw
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "glfw library 初始化失败！" << std::endl;
    exit(EXIT_FAILURE);
  }
  // 设置MSAA
  //glfwWindowHint(GLFW_SAMPLES, 4);
  // 初始化窗口位置
  glfwWindowHint(GLFW_POSITION_X, WINDOW_POSITION_X);
  glfwWindowHint(GLFW_POSITION_Y, WINDOW_POSITION_Y);
  // 创建窗口
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnMordenOpenGL", NULL, NULL);
  //glfwSetWindowPos(window, SCREEN_POSITION_X, SCREEN_POSITION_Y);
  glfwMakeContextCurrent(window);
  // 初始化gl3w
  gl3wInit();
  // 查询显卡支持的最大纹理单元数量
  GLint textureCount = 0;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureCount);
}

void ProjectRenderToTexture::createScene() {
  // 茶壶
  bool res_load_teapot = teapot.loadFromFile(pathTeapot);
  if (!res_load_teapot) {
    std::cout << "从路径" + pathTeapot + "加载模型失败！";
    exit(EXIT_FAILURE);
  }

  // 平面
  bool res_load_plane = plane.loadFromFile(pathPlane);
  if (!res_load_plane) {
    std::cout << "从路径" + pathPlane + "加载模型失败！";
    exit(EXIT_FAILURE);
  }

  // 相机
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
  // 渲染茶壶到framebuffer object
  drawTarget(frameBufferId);
}

void ProjectRenderToTexture::createFrameBuffer(){
  // 创建framebuffer obj
  glGenFramebuffers(1, &frameBufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

  // 创建纹理对象
  textureId = createTextureObject(textureWidth, textureHeight);
  // 连接纹理到帧缓存
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

  //创建渲染缓存
  GLuint renderBufferId;
  renderBufferId = createRenderBufferObject(textureWidth, textureHeight, GL_DEPTH_COMPONENT);
  // 连接渲染缓存到帧缓存
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    // error
    std::cout << "FRAMEBUFFER FAILURE!" << std::endl;
    exit(EXIT_FAILURE);
  }
}

GLuint ProjectRenderToTexture::createTextureObject(GLuint width, GLuint height) {
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
    GL_RGB, GL_UNSIGNED_BYTE, 0);

  // 设置参数
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  //glGenerateMipmap(GL_TEXTURE_2D);
  return textureId;
}

GLuint ProjectRenderToTexture::createRenderBufferObject(GLuint width, GLuint height, GLenum type) {
  GLuint renderBufferId;
  glGenRenderbuffers(1, &renderBufferId);
  glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
  glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);
  return renderBufferId;
}

void ProjectRenderToTexture::drawTarget(GLuint frameBufferId) {
  // 使用帧缓冲区
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
  // 清除设置
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  //glViewport(0, 0, textureWidth, textureHeight);
  //setClearBuffer1(frameBufferId, CLEAR_COLOR_GREY, &CLEAR_DEPTH);
  // 使用图形管线
  glUseProgram(shaderProgram);

  // 茶壶模型缩放设置
  scale = 0.1f;
  // 分配纹理采样单元
  //auto textureMgr = teapot.textureManager;
  //auto bufferIds = textureMgr.getTextureBuffers();
  //for (size_t i = 0; i < bufferIds.size(); i++) {
  //  GLuint textureId = bufferIds[i];
  //  glActiveTexture((GLenum)(GL_TEXTURE0 + i + sampleUnit_1));
  //  glBindTexture(GL_TEXTURE_2D, textureId);
  //  sampleUnit_1++;
  //}

  auto textureMgr = teapot.textureManager;
  auto bufferIds = textureMgr.getTextureBuffers();

  glActiveTexture((GLenum)(GL_TEXTURE0));
  glBindTexture(GL_TEXTURE_2D, bufferIds[0]);

  //glActiveTexture((GLenum)(GL_TEXTURE1));
  //glBindTexture(GL_TEXTURE_2D, bufferIds[1]);

  //glActiveTexture((GLenum)(GL_TEXTURE2));
  //glBindTexture(GL_TEXTURE_2D, textureId);

  // 计算shader所需变量值
  computeShaderData();
  // 修改shader变量
  passDataToShader1(shaderProgram);
  // 绘制茶壶
  teapot.draw();
}

void ProjectRenderToTexture::passDataToShader2(GLuint shaderProgram) {
  // 查询并修改全局变量
  ShaderProgramUtil programUtil(shaderProgram);

  bool resModifyMVP = programUtil.glModifyUniformMat44("v_u_mvp", modelViewProjection);
  if (!resModifyMVP) {
    //exit(EXIT_FAILURE);
  }

  bool resModifySample2d_0 = programUtil.glModifyUniformInt1("f_u_sample2d_diffuse", 2);
  if (!resModifySample2d_0) {
    //exit(EXIT_FAILURE);
  }

  //bool resModifySample2d_1 = programUtil.glModifyUniformInt1("f_u_sample2d_specular", 3);
  //if (!resModifySample2d_1) {
  //  //exit(EXIT_FAILURE);
  //}
}

void ProjectRenderToTexture::drawSecondPass() {
  // 使用window帧缓冲区
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // 清除设置
  //setClearBuffer1(0, CLEAR_COLOR_GREY, &CLEAR_DEPTH);
  glClearColor(0, 0, 0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  //glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  // 使用图形管线
  glUseProgram(shaderProgram);

  // 平面模型缩放设置
  scale = 1.0f;
  // 分配纹理采样单元
 /* auto textureMgr = plane.textureManager;
  auto bufferIds = textureMgr.getTextureBuffers();
  for (size_t i = 0; i < bufferIds.size(); i++) {
    GLuint textureId = bufferIds[i];
    glActiveTexture((GLenum)(GL_TEXTURE0 + i + sampleUnit_1));
    glBindTexture(GL_TEXTURE_2D, textureId);
    sampleUnit_1++;
  }

  glActiveTexture((GLenum)(GL_TEXTURE0 + textureUnitTarget));
  glBindTexture(GL_TEXTURE_2D, textureId);*/

  auto textureMgr = plane.textureManager;
  auto bufferIds = textureMgr.getTextureBuffers();

  glActiveTexture((GLenum)(GL_TEXTURE2));
  glBindTexture(GL_TEXTURE_2D, textureId);

  //glActiveTexture((GLenum)(GL_TEXTURE3));
  //glBindTexture(GL_TEXTURE_2D, bufferIds[1]);

  // 计算shader所需变量值
  computeShaderData();
  // 修改shader变量
  passDataToShader2(shaderProgram);
  // 绘制平面
  plane.draw();
}

void ProjectRenderToTexture::setClearBuffer1(GLuint frameBufferId, GLfloat* clearColor, GLfloat* clearDepth) {
  //glClearBufferfv(GL_COLOR, frameBufferId, clearColor);
  //glClearBufferfv(GL_DEPTH, frameBufferId, clearDepth);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
 /* glClearColor(0, 0, 0, 0); 
  glClear(GL_DEPTH_BUFFER_BIT);*/
  // 隐藏面消除
  glEnable(GL_DEPTH_TEST);
}

void ProjectRenderToTexture::computeShaderData() {
  // 计算mv, mvp, mvNormal
  glm::mat4 modelMatrix = glm::mat4(1.0f);

  modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, forward));

  modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1.0f, 0));

  modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

  glm::mat4 viewMatrix = camMain.getViewMatrix();

  glm::mat4 projectMatrix = glm::perspective(FOV, (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

  modelView = viewMatrix * modelMatrix;
  modelViewProjection = projectMatrix * viewMatrix * modelMatrix;

  // 法线在view空间的变换
  glm::mat4x4 modelViewForNormal44 = glm::transpose(glm::inverse(modelView));
  modelViewForNormal = modelViewForNormal44;

  // 计算dirLight。光线方向变换到view空间，不考虑平移，所以3*3矩阵就够了。
  glm::mat3 modelLight = glm::rotate(glm::mat4(1.0f), glm::radians(lightRotationSpeed), glm::vec3(1.0f, 0, -1.0f));
  glm::mat3 viewMatrix3 = viewMatrix;

  glm::vec3 dirLightCompute = viewMatrix3 * modelLight * light.lightPosition;
  dirLight = glm::normalize(dirLightCompute);
}

void ProjectRenderToTexture::passDataToShader1(GLuint shaderProgram) {
  // 查询并修改全局变量
  ShaderProgramUtil programUtil(shaderProgram);

  bool resModifyMVP = programUtil.glModifyUniformMat44("v_u_mvp", modelViewProjection);
  if (!resModifyMVP) {
    //exit(EXIT_FAILURE);
  }

  bool resModifySample2d_0 = programUtil.glModifyUniformInt1("f_u_sample2d_diffuse", 0);
  if (!resModifySample2d_0) {
    //exit(EXIT_FAILURE);
  }

  //bool resModifySample2d_1 = programUtil.glModifyUniformInt1("f_u_sample2d_specular", 1);
  //if (!resModifySample2d_1) {
  //  //exit(EXIT_FAILURE);
  //}
}

void ProjectRenderToTexture::cleanUp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
