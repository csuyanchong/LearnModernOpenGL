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
   if (key == GLFW_KEY_W) {
     //lightMoveVerticalSpeed += 0.1f;
     spotLight.transform.position.z -= 0.1f;
   }
   if (key == GLFW_KEY_S) {
     lightMoveVerticalSpeed -= 0.1f;
     spotLight.transform.position.z += 0.1f;
   }

   if (key == GLFW_KEY_A) {
     spotLight.transform.position.x -= 0.1f;
   }
   if (key == GLFW_KEY_D) {
     spotLight.transform.position.x += 0.1f;
     //camMain.moveRight(moveCamHorizenSpeed);
   }

   if (key == GLFW_KEY_Q) {
     spotLight.transform.position.y -= 0.1f;
   }
   if (key == GLFW_KEY_E) {
     spotLight.transform.position.y += 0.1f;
     //camMain.moveRight(moveCamHorizenSpeed);
   }

  if (key == GLFW_KEY_LEFT) {
    rotationTeapot -= 5.0f;
    teapotTransform.euler = glm::vec3(0, rotationTeapot, 0);
  }
  if (key == GLFW_KEY_RIGHT) {
    rotationTeapot += 5.0f;
    teapotTransform.euler = glm::vec3(0, rotationTeapot, 0);
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
    directionLight.transform.euler = glm::vec3(0, lightRotationSpeed, 0);
  }
  if (key == GLFW_KEY_C) {
    lightRotationSpeed += 1.0f;
    directionLight.transform.euler = glm::vec3(0, lightRotationSpeed, 0);
  }
}

void ProjectShadowMapping::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  //std::cout << "yoffset: " << yoffset << std::endl;
  //yoffset: 1为上，-1为下
  glm::vec3 dir = camMain.getLookDirection();
  if (yoffset > 0) {
    // 镜头拉近
    glm::vec3 positionNew = camMain.transform.position + dir * cameraMoveSpeed;
    camMain.transform.position = positionNew;
  }
  else {
    // 镜头拉远
    glm::vec3 positionNew = camMain.transform.position - dir * cameraMoveSpeed;
    camMain.transform.position = positionNew;
  }
}

void ProjectShadowMapping::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  // 鼠标右键按下
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    //std::cout << "mouse_button_right pressed!" << std::endl;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    lastPositionCursor.x = (float)xpos;
    lastPositionCursor.y = (float)ypos;

    isPressedMouseButtonRight = true;
  }
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
    //std::cout << "mouse_button_right released!" << std::endl;
    isPressedMouseButtonRight = false;
  }

  // 鼠标中键按下
  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
    isPressedMouseButtonMiddle = true;
  }
  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
    //std::cout << "mouse_button_right released!" << std::endl;
    isPressedMouseButtonMiddle = false;
  }
}

void ProjectShadowMapping::cursorCallBack(GLFWwindow* window, double xpos, double ypos) {
  //std::cout << xpos << "  " << ypos << std::endl;
  if (isPressedMouseButtonRight) {
    float speedRotate = 1.0f;
    if (xpos < lastPositionCursor.x) {
      camMain.transform.euler.y += speedRotate;
    }
    else if (xpos > lastPositionCursor.x) {
      camMain.transform.euler.y -= speedRotate;
    }

    if (ypos < lastPositionCursor.y) {
      camMain.transform.euler.x += speedRotate;
    }
    else if (ypos > lastPositionCursor.y) {
      camMain.transform.euler.x -= speedRotate;
    }
  }

  if (isPressedMouseButtonMiddle) {
    float speedMove = 2.0f;
    glm::vec2 position = glm::vec2((float)xpos, (float)ypos);
     glm::vec2 offset = position - lastPositionCursor;
    offset.x = offset.x / SCREEN_WIDTH * speedMove;
    offset.y = offset.y / SCREEN_HEIGHT * speedMove;

    camMain.transform.position += glm::vec3(offset, 0);

    /*glm::vec3 xAxis = glm::vec3(1.0f, 0, 0);
    glm::vec3 yAxis = glm::vec3(0, 1.0f, 0);

    glm::mat4 matrix = camMain.transform.getMatrix();
    glm::mat4 matrixInvert = glm::inverse(matrix);
    glm::mat3 matrixInvert33 = matrixInvert;
    glm::vec3 xAxisLocal = matrixInvert33 * xAxis;
    glm::vec3 yAxisLocal = matrixInvert33 * yAxis;

    glm::vec3 dirRight = glm::normalize(xAxisLocal);
    glm::vec3 dirUp = glm::normalize(yAxisLocal);

    glm::vec3 offsetLocalHorizen = -dirRight * offset.x;
    glm::vec3 offsetLocalVertical = dirUp * offset.y;

    glm::vec3 offsetLocal = offsetLocalHorizen + offsetLocalVertical;*/
    //camMain.transform.position += offsetLocal;
     
    //glm::vec3 dir = glm::vec3(.0f);
    //if (xpos < lastPositionCursor.x) {
    //  //camMain.transform.position.x += speedMove;
    //  dir.x = speedMove;
    //}
    //else if (xpos > lastPositionCursor.x) {
    //  //camMain.transform.position.x -= speedMove;
    //  dir.x = -speedMove;
    //}

    //if (ypos < lastPositionCursor.y) {
    //  //camMain.transform.position.y -= speedMove;
    //  dir.y = -speedMove;
    //}
    //else if (ypos > lastPositionCursor.y) {
    //  //camMain.transform.position.y += speedMove;
    //  dir.y = speedMove;
    //}

    //camMain.transform.position += dir;
  }

  lastPositionCursor.x = (float)xpos;
  lastPositionCursor.y = (float)ypos;
}

GLuint ProjectShadowMapping::createShaderProgram(const std::string& pathVert, const std::string& pathFrag) {
  std::vector<ShaderInfo> shaders;
  ShaderInfo vertInfo = {
    GL_VERTEX_SHADER, pathVert
  };

  ShaderInfo fragInfo = {
    GL_FRAGMENT_SHADER, pathFrag
  };

  shaders.push_back(vertInfo);
  shaders.push_back(fragInfo);

  GLuint shaderProgram = loadShader(shaders);
  return shaderProgram;
}

void ProjectShadowMapping::createWindow() {
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
  glfwSetWindowUserPointer(window, this);
  // 初始化gl3w
  gl3wInit();
  // 查询显卡支持的最大纹理单元数量
  GLint textureCount = 0;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureCount);
}

void ProjectShadowMapping::createScene() {
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

  // 立方体
  bool res_load_cube = cube.loadFromFile(pathCube);
  if (!res_load_cube) {
    std::cout << "从路径" + pathCube + "加载模型失败！";
    exit(EXIT_FAILURE);
  }
  
  // 光源模型
  bool res_load_light = light.loadFromFile(pathLight);
  if (!res_load_light) {
    std::cout << "从路径" + pathLight + "加载模型失败！";
    exit(EXIT_FAILURE);
  }

  // 平面变换设置
  planeTransrom.position = posPlane;
  planeTransrom.euler = glm::vec3(0, 0, 0);
  planeTransrom.scale = glm::vec3(scalePlane);

  // 茶壶变换设置
  teapotTransform.position = posTeapot;
  teapotTransform.euler = glm::vec3(0, rotationTeapot, 0);
  teapotTransform.scale = glm::vec3(scaleTeapot);

  // 相机
  camMain.transform.position = posCamera;
  camMain.transform.euler = eulerCamera;
  camMain.transform.scale = scaleCamera;
 /* camMain.setEyePosition(glm::vec3(0, 2, 3));
  camMain.setLookDirection(glm::vec3(0, 0, -1));
  camMain.setUpDirection(glm::vec3(0, 1, 0));*/

  // 灯光
  spotLight.transform.position = posSpotLight;
  spotLight.transform.euler = eulerSpotLight;
  spotLight.transform.scale = scaleSpotLight;
  spotLight.setAngleDegree(angleSpotLight);

  // 直射光
  directionLight.transform.position = glm::vec3(0);
  directionLight.transform.euler = glm::vec3(0, 0, 0);
  directionLight.transform.scale = glm::vec3(1);

  // 光源模型
  lightTransform.position = spotLight.transform.position;
  lightTransform.euler = spotLight.transform.euler;
  lightTransform.scale = glm::vec3(0.1f);
}

void ProjectShadowMapping::createRenderPipeline() {
  shaderSimpleProgram = createShaderProgram(pathSimpleVertShader, pathSimpleFragShader);

  shaderProgram = createShaderProgram(pathVertShader, pathFragShader);

  shaderLightProgram = createShaderProgram(pathLightVertShader, pathLightFragShader);
}

//GLuint ProjectShadowMapping::createShaderProgram(const std::string& pathVert, const std::string& pathFrag) {
//  std::vector<ShaderInfo> shaders;
//  ShaderInfo vertInfo = {
//    GL_VERTEX_SHADER, pathVert
//  };
//
//  ShaderInfo fragInfo = {
//    GL_FRAGMENT_SHADER, pathFrag
//  };
//
//  shaders.push_back(vertInfo);
//  shaders.push_back(fragInfo);
//
//  GLuint shaderProgram = loadShader(shaders);
//  return shaderProgram;
//}

void ProjectShadowMapping::bindInput() {
  auto callBackFunKey = [](GLFWwindow* window, int key, int code, int action, int mods) {
    static_cast<ProjectShadowMapping*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, code, action, mods);
    };

  auto callBackFunScroll = [](GLFWwindow* window, double xoffset, double yoffset) {
    static_cast<ProjectShadowMapping*>(glfwGetWindowUserPointer(window))->scrollCallback(window, xoffset, yoffset);
    };

  auto callBackFunMouseButton = [](GLFWwindow* window, int button, int action, int mods) {
    static_cast<ProjectShadowMapping*>(glfwGetWindowUserPointer(window))->mouseButtonCallback(window, button, action, mods);
    };

  auto callBackFunCursorPosition = [](GLFWwindow* window, double xpos, double ypos) {
    static_cast<ProjectShadowMapping*>(glfwGetWindowUserPointer(window))->cursorCallBack(window, xpos, ypos);
    };

  // 键盘事件回调
  glfwSetKeyCallback(window, callBackFunKey);

  // 鼠标滚轮事件回调
  glfwSetScrollCallback(window, callBackFunScroll);

  // 鼠标按键事件回调
  glfwSetMouseButtonCallback(window, callBackFunMouseButton);

  // 鼠标位置回调
  glfwSetCursorPosCallback(window, callBackFunCursorPosition);
}

void ProjectShadowMapping::mainLoop() {
  while (!glfwWindowShouldClose(window)) {
    draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void ProjectShadowMapping::draw() {
  // 保存相机参数
  Transform camTransform(camMain.transform);
  // 渲染深度信息贴图
  CameraInLightParam param = drawFirstPass();
  // 使用深度信息贴图
  drawSecondPass(param, camTransform);
}

CameraInLightParam ProjectShadowMapping::drawFirstPass() {
  CameraInLightParam res{};
  // 使用帧缓冲区
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
  // 清除设置
  //glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  //glViewport(0, 0, textureWidth, textureHeight);
  //setClearBuffer1(frameBufferId, CLEAR_COLOR_GREY, &CLEAR_DEPTH);

  // 使用一个简单的shader程序
  glUseProgram(shaderSimpleProgram);

  // 分配纹理采样单元
  glActiveTexture((GLenum)(GL_TEXTURE0 + shadowTextureUnit));
  glBindTexture(GL_TEXTURE_2D, textureId);
  //glBindTextureUnit(0, textureId); // GL4.5语法

  // 将相机移动到光源位置，绘制场景
  camMain.transform.position = spotLight.transform.position;
  camMain.transform.euler = spotLight.transform.euler;
  camMain.transform.scale = spotLight.transform.scale;

  // 计算相机参数
  glm::mat4 viewMatrix = camMain.getViewMatrix();

  // 计算投影矩阵, TODO... 透视封装到相机里
  glm::mat4 projectMatrix = glm::perspective(glm::radians(angleSpotLight), (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

  //// 计算shader所需参数
  //SimpleShaderParam planeData = computeModelShaderData(planeTransrom, viewMatrix, projectMatrix);
  //// 传递参数到shader
  //passModelDataToShaderProgram(shaderSimpleProgram, planeData);
  //// 绘制平面
  //plane.draw();

  // 绘制茶壶
  SimpleShaderParam teapotData = computeModelShaderData(teapotTransform, viewMatrix, projectMatrix);
  passModelDataToShaderProgram(shaderSimpleProgram, teapotData);
  teapot.draw();

  // 返回相关数据，供接下来使用
  res.viewMat = viewMatrix;
  res.projMat = projectMatrix;
  return res;
}

void ProjectShadowMapping::createFrameBuffer() {
  // 创建framebuffer obj
  glGenFramebuffers(1, &frameBufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

  textureWidth = SCREEN_WIDTH;
  textureHeight = SCREEN_HEIGHT;

  // 创建深度纹理
  textureId = createDepthTexture(textureWidth, textureHeight);

  // 配置
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);

  //// 创建纹理对象
  //textureId = createTextureObject(textureWidth, textureHeight);
  //// 连接纹理到帧缓存
  //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

  ////创建渲染缓存
  //GLuint renderBufferId;
  //renderBufferId = createRenderBufferObject(textureWidth, textureHeight, GL_DEPTH_COMPONENT);
  //// 连接渲染缓存到帧缓存
  //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    // error
    std::cout << "FRAMEBUFFER FAILURE!" << std::endl;
    exit(EXIT_FAILURE);
  }
}

GLuint ProjectShadowMapping::createDepthTexture(GLuint width, GLuint height) {
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
    GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

  // 设置参数
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  //glGenerateMipmap(GL_TEXTURE_2D);
  return textureId;
}

GLuint ProjectShadowMapping::createTextureObject(GLuint width, GLuint height) {
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
    GL_RGB, GL_UNSIGNED_BYTE, 0);

  // 设置参数
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

void ProjectShadowMapping::drawSecondPass(const CameraInLightParam& param,const Transform& camTransform) {
  // 使用window帧缓冲区
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // 清除设置
  glClearColor(0, 0, 0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  //glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
\
  // 使用图形管线
  glUseProgram(shaderProgram);

  // 相机恢复默认位置
  camMain.transform = camTransform;

  // 计算相机参数
  glm::mat4 viewMatrix = camMain.getViewMatrix();

  // 计算投影矩阵, TODO... 透视封装到相机里
  glm::mat4 projectMatrix = glm::perspective(glm::radians(FOV), (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

  //// 计算直射光光源参数
  //DirectionalLightShaderParam directionLightData = computeDirectionalLightShaderData(viewMatrix, directionLight);

  //// 传递直射光光源参数
  //passLightDataToShaderProgram(shaderProgram, directionLightData);

  // 计算聚光灯光源参数
  SpotLightShaderParam spotLightData = computeLightShaderData(viewMatrix, spotLight);

  // 传递光源参数
  passLightDataToShaderProgram(shaderProgram, spotLightData);

  // 计算平面shader参数
  ModelShaderParam planeData = computeModelShaderData(planeTransrom, viewMatrix, projectMatrix, colorPlane, shadowTextureUnit, param);

  // 传递shader参数
  passModelDataToShaderProgram(shaderProgram, planeData);

  // 绘制平面
  plane.draw();

  // 计算shader所需变量值
  ModelShaderParam teapotData = computeModelShaderData(teapotTransform, viewMatrix, projectMatrix, colorTeapot, shadowTextureUnit, param);

  // 传递shader参数
  passModelDataToShaderProgram(shaderProgram, teapotData);

  // 绘制茶壶
  teapot.draw();

  // 绘制光源
  glUseProgram(shaderLightProgram);

  lightTransform.position = spotLight.transform.position;
  lightTransform.euler = spotLight.transform.euler;
  // 计算shader所需参数
  LightModelShaderParam lightData = computeModelShaderData(lightTransform, viewMatrix, projectMatrix, colorLightModel_frame);

  // 传递参数到shader
  passModelDataToShaderProgram(shaderLightProgram, lightData);

  light.draw();
}

void ProjectShadowMapping::setClearBuffer1(GLuint frameBufferId, GLfloat* clearColor, GLfloat* clearDepth) {
  //glClearBufferfv(GL_COLOR, frameBufferId, clearColor);
  //glClearBufferfv(GL_DEPTH, frameBufferId, clearDepth);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* glClearColor(0, 0, 0, 0);
   glClear(GL_DEPTH_BUFFER_BIT);*/
   // 隐藏面消除
  glEnable(GL_DEPTH_TEST);
}

void ProjectShadowMapping::computeShaderData() {
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

SpotLightShaderParam ProjectShadowMapping::computeLightShaderData(const glm::mat4 viewMat, const SpotLight& light) {
  SpotLightShaderParam res{};
  
  //light.
  glm::mat4 modelMatrix = light.transform.getMatrix();
  glm::mat4 modelViewMatrix = viewMat * modelMatrix;
  glm::vec3 lookDirViewSpace = viewMat * glm::vec4(light.getLookDir(), 0);
  glm::vec4 positionView = modelViewMatrix * glm::vec4(0, 0, 0, 1.0f);

  // OpenGL用到的角度都是弧度
  res.angle = light.getAngleRadians();
  res.position = positionView;
  res.toward = glm::normalize(lookDirViewSpace);

  return res;
}

ModelShaderParam ProjectShadowMapping::computeModelShaderData(const Transform& trans, const glm::mat4 viewMat, const glm::mat4 projectMat, const glm::vec3 materialColor, GLuint shadowUnitIndex, CameraInLightParam camLightParam) {
  ModelShaderParam res{};

  // 计算模型mvp
  glm::mat4 modelMat = trans.getMatrix();
  glm::mat4 mvp = projectMat * viewMat * modelMat;

  // 计算模型mv
  glm::mat4 mv = viewMat * modelMat;

  // 计算法线mv
  glm::mat3 mvNormal = glm::transpose(glm::inverse(mv));

  // 计算相机在light位置的mvp
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
  glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f - bias));
  glm::mat4 mvpLight = translate * scale * camLightParam.projMat * camLightParam.viewMat * modelMat;

  res.color = materialColor;
  res.mv = mv;
  res.mvp = mvp;
  res.mvNormal = mvNormal;
  res.shadowSampleUnitId = shadowUnitIndex;
  res.mvpLight = mvpLight;

  return res;
}

void ProjectShadowMapping::computeShaderData(glm::vec3 pos, GLfloat rotation, GLfloat scale) {
  // 计算mvp
  glm::mat4 modelMatrix = glm::mat4(1.0f);

  modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1.0f, 0));

  modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

  modelMatrix = glm::translate(modelMatrix, pos);


  glm::mat4 viewMatrix = camMain.getViewMatrix();

  glm::mat4 projectMatrix = glm::perspective(glm::radians(FOV), (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

  modelView = viewMatrix * modelMatrix;
  modelViewProjection = projectMatrix * viewMatrix * modelMatrix;
  // 法线在view空间的变换
  modelViewForNormal = glm::transpose(glm::inverse(modelView));
}

void ProjectShadowMapping::passLightDataToShaderProgram(GLuint shaderProgram, const DirectionalLightShaderParam& data) {
  // 查询并修改全局变量
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

void ProjectShadowMapping::passLightDataToShaderProgram(GLuint shaderProgram, const SpotLightShaderParam& data) {
  // 查询并修改全局变量
  ShaderProgramUtil programUtil(shaderProgram);

  bool res = programUtil.glModifyUniformInt1("f_u_light_type", 1);
  if (!res) {
    //exit(EXIT_FAILURE);
  }

  // 修改mvp矩阵
  res = programUtil.glModifyUniformVec3("f_u_light_position", data.position);
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
  // 查询并修改全局变量
  ShaderProgramUtil programUtil(shaderProgram);

  // 修改mvp矩阵
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

  res = programUtil.glModifyUniformMat44("v_u_mv_light", data.mvpLight);
  if (!res) {
    //exit(EXIT_FAILURE);
  }

  // 修改颜色
  res = programUtil.glModifyUniformVec3("f_u_color", data.color);
  if (!res) {
    //exit(EXIT_FAILURE);
  }

  res = programUtil.glModifyUniformInt1("f_u_sample_shadow", data.shadowSampleUnitId);
  if (!res) {

  }
}

void ProjectShadowMapping::passModelDataToShaderProgram(GLuint shaderProgram, const SimpleShaderParam& data) {
  // 查询并修改全局变量
  ShaderProgramUtil programUtil(shaderProgram);

  // 修改mvp矩阵
  bool res = programUtil.glModifyUniformMat44("v_u_mvp", data.mvp);
  if (!res) {
    //exit(EXIT_FAILURE);
  }
}

void ProjectShadowMapping::passModelDataToShaderProgram(GLuint shaderProgram, const LightModelShaderParam& data) {
  // 查询并修改全局变量
  ShaderProgramUtil programUtil(shaderProgram);

  // 修改mvp矩阵
  bool res = programUtil.glModifyUniformMat44("v_u_mvp", data.mvp);
  if (!res) {
    //exit(EXIT_FAILURE);
  }

  // 修改颜色
  bool resModifyColor = programUtil.glModifyUniformVec3("f_u_color", data.colorFrame);
  if (!resModifyColor) {
    //exit(EXIT_FAILURE);
  }
}

//void passSimpleModelDataToShaderProgram(GLuint shaderProgram, const SimpleShaderParam& data) {
//  // 查询并修改全局变量
//  ShaderProgramUtil programUtil(shaderProgram);
//
//  // 修改mvp矩阵
//  bool res = programUtil.glModifyUniformMat44("v_u_mvp", data.mvp);
//  if (!res) {
//    //exit(EXIT_FAILURE);
//  }
//}

void ProjectShadowMapping::passPlaneDataToShader(GLuint _shaderProgram, glm::mat4 _mvp, glm::vec3 _color) {
  // 查询并修改全局变量
  ShaderProgramUtil programUtil(_shaderProgram);

  // 修改mvp矩阵
  bool resModifyMVP = programUtil.glModifyUniformMat44("v_u_mvp", _mvp);
  if (!resModifyMVP) {
    //exit(EXIT_FAILURE);
  }
  
  // 修改颜色
  bool resModifyColor = programUtil.glModifyUniformVec3("f_u_color", _color);
  if (!resModifyColor) {
    //exit(EXIT_FAILURE);
  }
}

void ProjectShadowMapping::passTeapotDataToShader(GLuint _shaderProgram, glm::mat4 _mvp, glm::vec3 _color) {
  // 查询并修改全局变量
  ShaderProgramUtil programUtil(_shaderProgram);

  // 修改mvp矩阵
  bool resModifyMVP = programUtil.glModifyUniformMat44("v_u_mvp", _mvp);
  if (!resModifyMVP) {
    //exit(EXIT_FAILURE);
  }

  // 修改颜色
  bool resModifyColor = programUtil.glModifyUniformVec3("f_u_color", _color);
  if (!resModifyColor) {
    //exit(EXIT_FAILURE);
  }
}

SimpleShaderParam ProjectShadowMapping::computeModelShaderData(const Transform& trans, const glm::mat4 viewMat, const glm::mat4 projectMat) {
  SimpleShaderParam res{};

  // 计算模型mvp
  glm::mat4 modelMat = trans.getMatrix();
  glm::mat4 mvp = projectMat * viewMat * modelMat;

  res.mvp = mvp;
  return res;
}

LightModelShaderParam ProjectShadowMapping::computeModelShaderData(const Transform& trans, const glm::mat4 viewMat, const glm::mat4 projectMat, const glm::vec3 color) {
  LightModelShaderParam res{};

  // 计算模型mvp
  glm::mat4 modelMat = trans.getMatrix();
  glm::mat4 mvp = projectMat * viewMat * modelMat;

  res.mvp = mvp;
  res.colorFrame = color;
  return res;
}

void ProjectShadowMapping::passDataToShader1(GLuint shaderProgram) {
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

void ProjectShadowMapping::cleanUp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}