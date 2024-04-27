// Platform
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

// Standard library header
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// Third-party library
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <lodepng/lodepng.h>

// Program src header
#include "shaderutil/ShaderUtil.h"
#include "camera/Camera.h"
#include "mesh/Mesh.h"
#include "model/Model.h"
#include "shaderutil/ShaderProgramUtil.h"
#include "project/basic/ProjectBasicDraw.h"
#include "project/shading/ProjectBlinnPhongShading.h"
#include "project/renderbuffer/ProjectRenderToTexture.h"


#ifdef _WIN32

#define minimizeConsoleWindow() ShowWindow(GetConsoleWindow(), SW_MINIMIZE)
#define showConsoleWindow() ShowWindow(GetConsoleWindow(), SW_NORMAL)

#endif // _WIN32

/* 屏幕宽度 */
static const int SCREEN_WIDTH = 1024;
/* 屏幕高度 */
static const int SCREEN_HEIGHT = 768;
/* 屏幕清除颜色 */
static const GLfloat CLEAR_COLOR[] = { 0.0f, 0.0f, 0.0f, 0.0f };
/* 屏幕深度缓存清除值 */
static const GLfloat CLEAR_DEPTH = 1.0f;
/* 窗口 */
GLFWwindow* window;

/* 顶点数组对象 */
GLuint vao = 0;
/* 顶点缓存对象 */
std::vector<GLuint> vbo(3);

/* 顶点位置 */
std::vector<GLfloat> verticesPosition;
/* 顶点颜色 */
std::vector<GLubyte> verticesColor;
/* 顶点索引 */
std::vector<GLuint> verticesIndex;

/* 图形管线 */
GLuint programPipeline;

/* 透视投影 */
GLfloat FOV = 45.0f;
GLfloat NEAR_CLIP_PLANE = 0.1f;
GLfloat FAR_CLIP_PLANE = 100.0f;

/* 模型变换 */
GLfloat rotation = 0;
GLfloat forward = 0;
GLfloat scale = 0.1f;

/* 主摄像机 */
Camera camMain(
  glm::vec3(1, 2, 3),
  glm::vec3(0, 0, -1),
  glm::vec3(0, 1.0f, 0)
);

GLfloat moveCamVerticalSpeed = 0.1f;
GLfloat moveCamHorizenSpeed = 0.1f;

/* 模型文件地址 */
const std::string MODELS_DIR = "./data/models/objmodel/";
std::string nameModel = "teapot.obj";
std::string pathModel = MODELS_DIR + nameModel;

/* 茶壶模型 */
Model teapot;

/* 平面模型 */
Model plane;
std::string namePlane = "plane.obj";
std::string pathPlane = MODELS_DIR + namePlane;

/* 模型贴图 */
std::string nameDiffuseTexture = "brick.png";
std::string pathDiffuseTexture = MODELS_DIR + nameDiffuseTexture;

//
//Texture diffuseTexture = {
//  pathDiffuseTexture,
//  0,
//  0,
//  {}
//};


/* 模型材质颜色 */
glm::vec3 materialColor = glm::vec3(1.0f, 0, 0);

/* shader文件地址 */

std::string SHADER_DIR = "./data/shaders/";
std::string blinn_phong_vert_shader = "blinn_phong.vert";
std::string blinn_phong_frag_shader = "blinn_phong.frag";

std::string texture_map_vert_shader = "texture_map.vert";
std::string texture_map_frag_shader = "texture_map.frag";

/* 当前使用的shader */
std::string curVertShader = SHADER_DIR + texture_map_vert_shader;
std::string curFragShader = SHADER_DIR + texture_map_frag_shader;

/* 平行光Dirction light光照模型 */
struct DirectionLight {
  glm::vec3 lightPosition;
  glm::vec3 lightColor;
};

DirectionLight light = {
  glm::vec3(1.0f, 1.0f, 1.0f),
  glm::vec3(1.0f, 1.0f, 1.0f)
};

/* 材质模型 */
struct BlinnMaterial {
  glm::vec3 ka;
  glm::vec3 kd;
  glm::vec3 ks;
  GLfloat alpha;
};

//BlinnMaterial material = {
//  glm::vec3(0.3451f, 0.6941f, 0.1059f),
//  glm::vec3(0.3451f, 0.6941f, 0.1059f),
//  glm::vec3(1.0f, 1.0f, 1.0f),
//  64.0f
//};

BlinnMaterial material = {
  glm::vec3(1.0f, 0, 0),
  glm::vec3(1.0f, 0, 0),
  glm::vec3(1.0f, 1.0f, 1.0f),
  64.0f
};


/* 计算参数 */
glm::mat4 modelView;
glm::mat4 modelViewProjection;
glm::mat3 modelViewForNormal;

glm::vec3 dirLight;

/* 灯光旋转参数 */
GLfloat lightRotationSpeed = 0;

void uniformBlinnPhongShader(ShaderProgramUtil programUtil) {
  bool resModifyMVP = programUtil.glModifyUniformMat44("u_mvp", modelViewProjection);
  if (!resModifyMVP) {
    //exit(EXIT_FAILURE);
  }

  bool resModifyMV = programUtil.glModifyUniformMat44("u_mv", modelView);
  if (!resModifyMV) {
    //exit(EXIT_FAILURE);
  }

  bool resModifyMVNormal = programUtil.glModifyUniformMat33("u_normal", modelViewForNormal);
  if (!resModifyMVNormal) {
    //exit(EXIT_FAILURE);
  }

  bool resModifyKa = programUtil.glModifyUniformVec3("u_ka", material.ka);
  if (!resModifyKa) {
    //exit(EXIT_FAILURE);
  }

  resModifyKa = programUtil.glModifyUniformVec3("u_kd", material.kd);
  if (!resModifyKa) {
    //exit(EXIT_FAILURE);
  }

  resModifyKa = programUtil.glModifyUniformVec3("u_ks", material.ks);
  if (!resModifyKa) {
    //exit(EXIT_FAILURE);
  }

  resModifyKa = programUtil.glModifyUniformFloat("u_alpha", material.alpha);
  if (!resModifyKa) {
    //exit(EXIT_FAILURE);
  }

  bool resModifyLight = programUtil.glModifyUniformVec3("u_dirLight", dirLight);
  if (!resModifyLight) {
    //exit(EXIT_FAILURE);
  }
}

void uniformTextureMapShader(ShaderProgramUtil programUtil) {

  bool resModifyMVP = programUtil.glModifyUniformMat44("v_u_mvp", modelViewProjection);
  if (!resModifyMVP) {
    //exit(EXIT_FAILURE);
  }

  bool resModifySample2d_0 = programUtil.glModifyUniformInt1("f_u_sample2d_0", 0);
  if (!resModifySample2d_0) {
    //exit(EXIT_FAILURE);
  }

  bool resModifySample2d_1 = programUtil.glModifyUniformInt1("f_u_sample2d_1", 1);
  if (!resModifySample2d_1) {
    //exit(EXIT_FAILURE);
  }

}

void preCompute() {
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

void clearSetting() {
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

void preDraw() {
  clearSetting();
  // 使用图形管线
  glUseProgram(programPipeline);

  //plane.preDraw();

  // 预计算着色参数
  preCompute();

  // 查询并修改全局变量
  ShaderProgramUtil programUtil(programPipeline);

  //uniformBlinnPhongShader(programUtil);
  uniformTextureMapShader(programUtil);
}

void draw() {
  plane.draw();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  if (key == GLFW_KEY_W) {
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
  }

  if (key == GLFW_KEY_LEFT) {
    rotation -= 5.0f;
  }
  if (key == GLFW_KEY_RIGHT) {
    rotation += 5.0f;
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
  }
  if (key == GLFW_KEY_C) {
    lightRotationSpeed += 1.0f;
  }
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void query() {
  std::cout << "Hardware capability query" << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  // 查询默认framebuffer id
  GLint oriBuffer = 0;
  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &oriBuffer);
  std::cout << "--Default framebuffer id: " << oriBuffer << std::endl;

  // 查询多重采样MSAA所支持的最大采样数
  GLint maxSamples = 0;
  glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
  std::cout << "--Supported max sample count: " << maxSamples << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}

void initSetup() {
  // 初始化glfw
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "glfw library 初始化失败！" << std::endl;
    exit(EXIT_FAILURE);
  }
  // 设置MSAA
  glfwWindowHint(GLFW_SAMPLES, 4);
  // 创建窗口
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnMordenOpenGL", NULL, NULL);
  glfwMakeContextCurrent(window);
  // 初始化gl3w
  gl3wInit();
  // 键盘事件回调
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
  // 查询
  query();
}

void createVertexData() {
  // 加载模型文件
  bool resLoadModel = teapot.loadFromFile(pathModel);
  if (!resLoadModel) {
    std::cout << "从路径" + pathModel + "加载模型失败！";
    exit(EXIT_FAILURE);
  }

  bool resLoadPlane = plane.loadFromFile(pathPlane);
  if (!resLoadPlane) {
    std::cout << "从路径" + pathPlane + "加载模型失败！";
    exit(EXIT_FAILURE);
  }
}

void createGraphicPipeline() {
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

void renderTeapotToTexture() {

}

void firstPass() {
  // 第一遍，渲染茶壶到纹理
  renderTeapotToTexture();
}

void drawPlane() {

}

void secondPass() {
  // 第二遍，使用第一遍渲染的茶壶纹理，显示到平面模型上。
  drawPlane();
}

void mainLoop() {
  while (!glfwWindowShouldClose(window))
  {
    firstPass();
    secondPass();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void cleanUp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

int main(int argc, char* argv[]) {
  // TODO... 提供界面，显示项目列表，用户可以选择不同的项目。
  while (true) {
    std::cout << "当前项目列表如下：" << std::endl;
    std::cout << "1: basic" << std::endl;
    std::cout << "2: blinn phong shading" << std::endl;
    std::cout << "3: textures" << std::endl;
    std::cout << "4: render to texture" << std::endl;
    std::cout << "100: 清除控制台" << std::endl;

    std::cout << "\n" << std::endl;
    std::cout << "请输入项目编号：" << std::endl;

    int inputNum = 0;
    std::cin >> inputNum;

    switch (inputNum) {
    case 1: {
      std::cout << "选择了1: basic" << std::endl;
      minimizeConsoleWindow();
      ProjectBasicDraw proj;
      proj.run();
      break;
    }

    case 2: {
      std::cout << "选择了2: blinn phong shading" << std::endl;
      minimizeConsoleWindow();
      ProjectBlinnPhongShading proj;
      proj.run();
      break;
    }

    case 3: {
      std::cout << "选择了3: textures" << std::endl;
      minimizeConsoleWindow();
      // TODO...
      break;
    }

    case 4: {
      std::cout << "选择了4: render to texture" << std::endl;
      minimizeConsoleWindow();
      ProjectRenderToTexture proj;
      proj.run();
      break;
    }

    case 100: {
      system("cls");
      break;
    }

    default:
      break;
    }

    showConsoleWindow();
  }
  return 0;
}