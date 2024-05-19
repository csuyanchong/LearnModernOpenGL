#include "ProjectShadowMapping.h"

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
#include "../../texture/GLTextureManager.h"

ProjectShadowMapping::ProjectShadowMapping() {
  // ��������
  createWindow();
  // ��������
  createScene();
  // ������Ⱦ����
  createRenderPipeline();
  // ������
  bindInput();
  // ����framebuffer
  createFrameBuffer();
}

ProjectShadowMapping::~ProjectShadowMapping() {
  cleanUp();
}

void ProjectShadowMapping::run() {
  mainLoop();
}

void ProjectShadowMapping::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  /* if (key == GLFW_KEY_W) {
     camMain.moveForward(moveCamVerticalSpeed);
   }
   if (key == GLFW_KEY_S) {
     camMain.moveBack(moveCamVerticalSpeed);
   }

   if (key == GLFW_KEY_A) {
     camMain.moveLeft(moveCamHorizenSpeed);
   }
   if (key == GLFW_KEY_D) {
     camMain.moveRight(moveCamHorizenSpeed);
   }*/

  if (key == GLFW_KEY_LEFT) {
    rotationTeapot -= 5.0f;
    teapotTransform.euler = glm::vec3(0, glm::radians(rotationTeapot), 0);
  }
  if (key == GLFW_KEY_RIGHT) {
    rotationTeapot += 5.0f;
    teapotTransform.euler = glm::vec3(0, glm::radians(rotationTeapot), 0);
  }
  if (key == GLFW_KEY_UP) {
    forward += 0.1f;
  }
  if (key == GLFW_KEY_DOWN) {
    forward -= 0.1f;
  }
  if (key == GLFW_KEY_T) {
    scale += 0.001f;
    scale = fmin(scale, 0.02f);
  }
  if (key == GLFW_KEY_G) {
    scale -= 0.001f;
    scale = fmaxf(scale, 0.002f);
  }
  if (key == GLFW_KEY_N)
  {
    scale = 0.01f;
  }

  if (key == GLFW_KEY_Z) {
    lightRotationSpeed -= 1.0f;
    directionLight.transform.euler = glm::vec3(0, glm::radians(lightRotationSpeed), 0);
  }
  if (key == GLFW_KEY_C) {
    lightRotationSpeed += 1.0f;
    directionLight.transform.euler = glm::vec3(0, glm::radians(lightRotationSpeed), 0);
  }
}

void ProjectShadowMapping::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  //std::cout << "yoffset: " << yoffset << std::endl;
  // yoffset: 1Ϊ�ϣ�-1Ϊ��
  glm::vec3 dir = camMain.getLookDirection();
  if (yoffset > 0) {
    // ��ͷ����
    glm::vec3 positionNew = camMain.transform.position + dir * cameraMoveSpeed;
    camMain.transform.position = positionNew;
  }
  else {
    // ��ͷ��Զ
    glm::vec3 positionNew = camMain.transform.position - dir * cameraMoveSpeed;
    camMain.transform.position = positionNew;
  }
}

void ProjectShadowMapping::createWindow() {
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
  glfwSetWindowUserPointer(window, this);
  // ��ʼ��gl3w
  gl3wInit();
  // ��ѯ�Կ�֧�ֵ��������Ԫ����
  GLint textureCount = 0;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureCount);
}

void ProjectShadowMapping::createScene() {
  // ���
  bool res_load_teapot = teapot.loadFromFile(pathTeapot);
  if (!res_load_teapot) {
    std::cout << "��·��" + pathTeapot + "����ģ��ʧ�ܣ�";
    exit(EXIT_FAILURE);
  }

  // ƽ��
  bool res_load_plane = plane.loadFromFile(pathPlane);
  if (!res_load_plane) {
    std::cout << "��·��" + pathPlane + "����ģ��ʧ�ܣ�";
    exit(EXIT_FAILURE);
  }

  // ������
  bool res_load_cube = cube.loadFromFile(pathCube);
  if (!res_load_cube) {
    std::cout << "��·��" + pathCube + "����ģ��ʧ�ܣ�";
    exit(EXIT_FAILURE);
  }
  
  // ƽ��任����
  planeTransrom.position = posPlane;
  planeTransrom.euler = glm::vec3(0, 0, 0);
  planeTransrom.scale = glm::vec3(scalePlane);

  // ����任����
  teapotTransform.position = posTeapot;
  teapotTransform.euler = glm::vec3(0, glm::radians(rotationTeapot), 0);
  teapotTransform.scale = glm::vec3(scaleTeapot);

  // ���
  camMain.transform.position = posCamera;
  camMain.transform.euler = eulerCamera;
  camMain.transform.scale = scaleCamera;
 /* camMain.setEyePosition(glm::vec3(0, 2, 3));
  camMain.setLookDirection(glm::vec3(0, 0, -1));
  camMain.setUpDirection(glm::vec3(0, 1, 0));*/

  // �ƹ�
  light.position = glm::vec3(2.0f, 2.0f, 2.0f);
  light.lookPoint = glm::vec3(0, 0, 0);
  light.angle = 60;

  // ֱ���
  directionLight.transform.position = glm::vec3(0);
  directionLight.transform.euler = glm::vec3(0, 0, 0);
  directionLight.transform.scale = glm::vec3(1);
}

void ProjectShadowMapping::createRenderPipeline() {
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

void ProjectShadowMapping::bindInput() {
  auto callBackFunKey = [](GLFWwindow* window, int key, int code, int action, int mods) {
    static_cast<ProjectShadowMapping*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, code, action, mods);
    };

  auto callBackFunScroll = [](GLFWwindow* window, double xoffset, double yoffset) {
    static_cast<ProjectShadowMapping*>(glfwGetWindowUserPointer(window))->scrollCallback(window, xoffset, yoffset);
    };

  // �����¼��ص�
  glfwSetKeyCallback(window, callBackFunKey);

  // �������¼��ص�
  glfwSetScrollCallback(window, callBackFunScroll);
}

void ProjectShadowMapping::mainLoop() {
  while (!glfwWindowShouldClose(window)) {
    draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void ProjectShadowMapping::draw() {
  //drawFirstPass();
  drawSecondPass();
}

void ProjectShadowMapping::drawFirstPass() {
  // ��Ⱦ�����framebuffer object
  drawTarget(frameBufferId);
}

void ProjectShadowMapping::createFrameBuffer() {
  // ����framebuffer obj
  glGenFramebuffers(1, &frameBufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

  // �����������
  textureId = createTextureObject(textureWidth, textureHeight);
  // ��������֡����
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

  //������Ⱦ����
  GLuint renderBufferId;
  renderBufferId = createRenderBufferObject(textureWidth, textureHeight, GL_DEPTH_COMPONENT);
  // ������Ⱦ���浽֡����
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    // error
    std::cout << "FRAMEBUFFER FAILURE!" << std::endl;
    exit(EXIT_FAILURE);
  }
}

GLuint ProjectShadowMapping::createTextureObject(GLuint width, GLuint height) {
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
    GL_RGB, GL_UNSIGNED_BYTE, 0);

  // ���ò���
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  //glGenerateMipmap(GL_TEXTURE_2D);
  return textureId;
}

GLuint ProjectShadowMapping::createRenderBufferObject(GLuint width, GLuint height, GLenum type) {
  GLuint renderBufferId;
  glGenRenderbuffers(1, &renderBufferId);
  glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
  glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);
  return renderBufferId;
}

void ProjectShadowMapping::drawTarget(GLuint frameBufferId) {
  // ʹ��֡������
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
  // �������
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  //glViewport(0, 0, textureWidth, textureHeight);
  //setClearBuffer1(frameBufferId, CLEAR_COLOR_GREY, &CLEAR_DEPTH);
  // ʹ��ͼ�ι���
  glUseProgram(shaderProgram);

  // ���ģ����������
  scale = 0.1f;
  // �������������Ԫ
  //auto textureMgr = teapot.textureManager;
  //auto bufferIds = textureMgr.getTextureBuffers();
  //for (size_t i = 0; i < bufferIds.size(); i++) {
  //  GLuint textureId = bufferIds[i];
  //  glActiveTexture((GLenum)(GL_TEXTURE0 + i + sampleUnit_1));
  //  glBindTexture(GL_TEXTURE_2D, textureId);
  //  sampleUnit_1++;
  //}

  GLTextureManager& textureMgr = teapot.textureManager;
  std::vector<GLuint> bufferIds = textureMgr.getTextureBuffers();

  glActiveTexture((GLenum)(GL_TEXTURE0));
  glBindTexture(GL_TEXTURE_2D, bufferIds[0]);

  //glActiveTexture((GLenum)(GL_TEXTURE1));
  //glBindTexture(GL_TEXTURE_2D, bufferIds[1]);

  //glActiveTexture((GLenum)(GL_TEXTURE2));
  //glBindTexture(GL_TEXTURE_2D, textureId);

  // ����shader�������ֵ
  computeShaderData();
  // �޸�shader����
  passDataToShader1(shaderProgram);
  // ���Ʋ��
  teapot.draw();
}

void ProjectShadowMapping::passDataToShader2(GLuint shaderProgram) {
  // ��ѯ���޸�ȫ�ֱ���
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

void ProjectShadowMapping::drawSecondPass() {
  // ʹ��window֡������
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // �������
  glClearColor(0, 0, 0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  //glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
\
  // ʹ��ͼ�ι���
  glUseProgram(shaderProgram);

  // �����������
  glm::mat4 viewMatrix = camMain.getViewMatrix();

  // ����ͶӰ����, TODO... ͸�ӷ�װ�������
  glm::mat4 projectMatrix = glm::perspective(glm::radians(FOV), (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

  // ����ֱ����Դ����
  DirectionalLightShaderParam directionLightData = computeDirectionalLightShaderData(viewMatrix, directionLight);

  // ����ֱ����Դ����
  passLightDataToShaderProgram(shaderProgram, directionLightData);

  //// �����Դ����
  //LightShaderParam lightData = computeLightShaderData(viewMatrix, light);

  //// ���ݹ�Դ����
  //passLightDataToShaderProgram(shaderProgram, lightData);

  // ����ƽ��shader����
  ModelShaderParam planeData = computeModelShaderData(planeTransrom, viewMatrix, projectMatrix, colorPlane);

  // ����shader����
  passModelDataToShaderProgram(shaderProgram, planeData);

  // ����ƽ��
  plane.draw();

  // ����shader�������ֵ
  ModelShaderParam teapotData = computeModelShaderData(teapotTransform, viewMatrix, projectMatrix, colorTeapot);

  // ����shader����
  passModelDataToShaderProgram(shaderProgram, teapotData);

  // ���Ʋ��
  teapot.draw();
}

void ProjectShadowMapping::setClearBuffer1(GLuint frameBufferId, GLfloat* clearColor, GLfloat* clearDepth) {
  //glClearBufferfv(GL_COLOR, frameBufferId, clearColor);
  //glClearBufferfv(GL_DEPTH, frameBufferId, clearDepth);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* glClearColor(0, 0, 0, 0);
   glClear(GL_DEPTH_BUFFER_BIT);*/
   // ����������
  glEnable(GL_DEPTH_TEST);
}

void ProjectShadowMapping::computeShaderData() {
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

  //glm::vec3 dirLightCompute = viewMatrix3 * modelLight * light.lightPosition;
  //dirLight = glm::normalize(dirLightCompute);
}

DirectionalLightShaderParam ProjectShadowMapping::computeDirectionalLightShaderData(const glm::mat4 viewMat, const DirectionalLight& light) {
  DirectionalLightShaderParam res;
  glm::vec3 direction = light.getDirection();
  glm::vec3 directionView = viewMat * glm::vec4(direction, 0);
  res.direction = directionView;
  return res;
}

LightShaderParam ProjectShadowMapping::computeLightShaderData(const glm::mat4 viewMat, const SpotLight& light) {
  LightShaderParam res{};
  
  //light.
  glm::mat4 modelMatrix = light.getMatrix();
  glm::mat4 modelViewMatrix = viewMat * modelMatrix;
  glm::vec3 lookDirViewSpace = (glm::mat3)modelViewMatrix * light.getLookDir();
  glm::vec4 positionView = modelViewMatrix * glm::vec4(light.position, 1.0f);

  res.angle = light.angle;
  res.position = positionView;
  res.toward = lookDirViewSpace;

  return res;
}

ModelShaderParam ProjectShadowMapping::computeModelShaderData(const Transform& trans, const glm::mat4 viewMat, const glm::mat4 projectMat, const glm::vec3 materialColor) {
  ModelShaderParam res{};

  // ����ģ��mvp
  glm::mat4 modelMat = trans.getMatrix();
  glm::mat4 mvp = projectMat * viewMat * modelMat;

  // ����ģ��mv
  glm::mat4 mv = viewMat * modelMat;

  // ���㷨��mv
  glm::mat3 mvNormal = glm::transpose(glm::inverse(mv));

  res.color = materialColor;
  res.mv = mv;
  res.mvp = mvp;
  res.mvNormal = mvNormal;

  return res;
}

void ProjectShadowMapping::computeShaderData(glm::vec3 pos, GLfloat rotation, GLfloat scale) {
  // ����mvp
  glm::mat4 modelMatrix = glm::mat4(1.0f);

  modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1.0f, 0));

  modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

  modelMatrix = glm::translate(modelMatrix, pos);


  glm::mat4 viewMatrix = camMain.getViewMatrix();

  glm::mat4 projectMatrix = glm::perspective(glm::radians(FOV), (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

  modelView = viewMatrix * modelMatrix;
  modelViewProjection = projectMatrix * viewMatrix * modelMatrix;
  // ������view�ռ�ı任
  modelViewForNormal = glm::transpose(glm::inverse(modelView));
}

void ProjectShadowMapping::passLightDataToShaderProgram(GLuint shaderProgram, const DirectionalLightShaderParam& data) {
  // ��ѯ���޸�ȫ�ֱ���
  ShaderProgramUtil programUtil(shaderProgram);

  bool res = programUtil.glModifyUniformInt1("f_u_light_type", 0);
  if (!res) {
    //exit(EXIT_FAILURE);
  }

  res = programUtil.glModifyUniformVec3("f_u_light_directional_direction", data.direction);
  if (!res) {
    //exit(EXIT_FAILURE);
  }
}

void ProjectShadowMapping::passLightDataToShaderProgram(GLuint shaderProgram, const LightShaderParam& data) {
  // ��ѯ���޸�ȫ�ֱ���
  ShaderProgramUtil programUtil(shaderProgram);

  // �޸�mvp����
  bool res = programUtil.glModifyUniformVec3("f_u_light_position", data.position);
  if (!res) {
    //exit(EXIT_FAILURE);
  }

  res = programUtil.glModifyUniformVec3("f_u_light_toward", data.toward);
  if (!res) {
    //exit(EXIT_FAILURE);
  }

  res = programUtil.glModifyUniformFloat("f_u_light_angle", data.angle);
  if (!res) {
    //exit(EXIT_FAILURE);
  }
}

void ProjectShadowMapping::passModelDataToShaderProgram(GLuint shaderProgram, const ModelShaderParam& data) {
  // ��ѯ���޸�ȫ�ֱ���
  ShaderProgramUtil programUtil(shaderProgram);

  // �޸�mvp����
  bool res = programUtil.glModifyUniformMat44("v_u_mvp", data.mvp);
  if (!res) {
    //exit(EXIT_FAILURE);
  }

  res = programUtil.glModifyUniformMat44("v_u_mv", data.mv);
  if (!res) {
    //exit(EXIT_FAILURE);
  }

  res = programUtil.glModifyUniformMat33("v_u_mv_normal", data.mvNormal);
  if (!res) {
    //exit(EXIT_FAILURE);
  }

  // �޸���ɫ
  res = programUtil.glModifyUniformVec3("f_u_color", data.color);
  if (!res) {
    //exit(EXIT_FAILURE);
  }
}

void ProjectShadowMapping::passPlaneDataToShader(GLuint _shaderProgram, glm::mat4 _mvp, glm::vec3 _color) {
  // ��ѯ���޸�ȫ�ֱ���
  ShaderProgramUtil programUtil(_shaderProgram);

  // �޸�mvp����
  bool resModifyMVP = programUtil.glModifyUniformMat44("v_u_mvp", _mvp);
  if (!resModifyMVP) {
    //exit(EXIT_FAILURE);
  }
  
  // �޸���ɫ
  bool resModifyColor = programUtil.glModifyUniformVec3("f_u_color", _color);
  if (!resModifyColor) {
    //exit(EXIT_FAILURE);
  }
}

void ProjectShadowMapping::passTeapotDataToShader(GLuint _shaderProgram, glm::mat4 _mvp, glm::vec3 _color) {
  // ��ѯ���޸�ȫ�ֱ���
  ShaderProgramUtil programUtil(_shaderProgram);

  // �޸�mvp����
  bool resModifyMVP = programUtil.glModifyUniformMat44("v_u_mvp", _mvp);
  if (!resModifyMVP) {
    //exit(EXIT_FAILURE);
  }

  // �޸���ɫ
  bool resModifyColor = programUtil.glModifyUniformVec3("f_u_color", _color);
  if (!resModifyColor) {
    //exit(EXIT_FAILURE);
  }
}

void ProjectShadowMapping::passDataToShader1(GLuint shaderProgram) {
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

  //bool resModifySample2d_1 = programUtil.glModifyUniformInt1("f_u_sample2d_specular", 1);
  //if (!resModifySample2d_1) {
  //  //exit(EXIT_FAILURE);
  //}
}

void ProjectShadowMapping::cleanUp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}