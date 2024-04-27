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

/* ��Ļ��� */
static const int SCREEN_WIDTH = 1024;
/* ��Ļ�߶� */
static const int SCREEN_HEIGHT = 768;
/* ��Ļ�����ɫ */
static const GLfloat CLEAR_COLOR[] = { 0.0f, 0.0f, 0.0f, 0.0f };
/* ��Ļ��Ȼ������ֵ */
static const GLfloat CLEAR_DEPTH = 1.0f;
/* ���� */
GLFWwindow* window;

/* ����������� */
GLuint vao = 0;
/* ���㻺����� */
std::vector<GLuint> vbo(3);

/* ����λ�� */
std::vector<GLfloat> verticesPosition;
/* ������ɫ */
std::vector<GLubyte> verticesColor;
/* �������� */
std::vector<GLuint> verticesIndex;

/* ͼ�ι��� */
GLuint programPipeline;

/* ͸��ͶӰ */
GLfloat FOV = 45.0f;
GLfloat NEAR_CLIP_PLANE = 0.1f;
GLfloat FAR_CLIP_PLANE = 100.0f;

/* ģ�ͱ任 */
GLfloat rotation = 0;
GLfloat forward = 0;
GLfloat scale = 0.1f;

/* ������� */
Camera camMain(
  glm::vec3(1, 2, 3),
  glm::vec3(0, 0, -1),
  glm::vec3(0, 1.0f, 0)
);

GLfloat moveCamVerticalSpeed = 0.1f;
GLfloat moveCamHorizenSpeed = 0.1f;

/* ģ���ļ���ַ */
const std::string MODELS_DIR = "./data/models/objmodel/";
std::string nameModel = "teapot.obj";
std::string pathModel = MODELS_DIR + nameModel;

/* ���ģ�� */
Model teapot;

/* ƽ��ģ�� */
Model plane;
std::string namePlane = "plane.obj";
std::string pathPlane = MODELS_DIR + namePlane;

/* ģ����ͼ */
std::string nameDiffuseTexture = "brick.png";
std::string pathDiffuseTexture = MODELS_DIR + nameDiffuseTexture;

//
//Texture diffuseTexture = {
//  pathDiffuseTexture,
//  0,
//  0,
//  {}
//};


/* ģ�Ͳ�����ɫ */
glm::vec3 materialColor = glm::vec3(1.0f, 0, 0);

/* shader�ļ���ַ */

std::string SHADER_DIR = "./data/shaders/";
std::string blinn_phong_vert_shader = "blinn_phong.vert";
std::string blinn_phong_frag_shader = "blinn_phong.frag";

std::string texture_map_vert_shader = "texture_map.vert";
std::string texture_map_frag_shader = "texture_map.frag";

/* ��ǰʹ�õ�shader */
std::string curVertShader = SHADER_DIR + texture_map_vert_shader;
std::string curFragShader = SHADER_DIR + texture_map_frag_shader;

/* ƽ�й�Dirction light����ģ�� */
struct DirectionLight {
  glm::vec3 lightPosition;
  glm::vec3 lightColor;
};

DirectionLight light = {
  glm::vec3(1.0f, 1.0f, 1.0f),
  glm::vec3(1.0f, 1.0f, 1.0f)
};

/* ����ģ�� */
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


/* ������� */
glm::mat4 modelView;
glm::mat4 modelViewProjection;
glm::mat3 modelViewForNormal;

glm::vec3 dirLight;

/* �ƹ���ת���� */
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

void clearSetting() {
  // �����ɫ����Ȼ���
  glClearBufferfv(GL_COLOR, 0, CLEAR_COLOR);
  glClearBufferfv(GL_DEPTH, 0, &CLEAR_DEPTH);

  // ���������޳�
  //glEnable(GL_CULL_FACE);
  //glFrontFace(GL_CCW);
  //glCullFace(GL_BACK);

  // ����������
  glEnable(GL_DEPTH_TEST);
  //// ����MSAA
  //glEnable(GL_MULTISAMPLE);
}

void preDraw() {
  clearSetting();
  // ʹ��ͼ�ι���
  glUseProgram(programPipeline);

  //plane.preDraw();

  // Ԥ������ɫ����
  preCompute();

  // ��ѯ���޸�ȫ�ֱ���
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
  // ��ѯĬ��framebuffer id
  GLint oriBuffer = 0;
  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &oriBuffer);
  std::cout << "--Default framebuffer id: " << oriBuffer << std::endl;

  // ��ѯ���ز���MSAA��֧�ֵ���������
  GLint maxSamples = 0;
  glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
  std::cout << "--Supported max sample count: " << maxSamples << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}

void initSetup() {
  // ��ʼ��glfw
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "glfw library ��ʼ��ʧ�ܣ�" << std::endl;
    exit(EXIT_FAILURE);
  }
  // ����MSAA
  glfwWindowHint(GLFW_SAMPLES, 4);
  // ��������
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnMordenOpenGL", NULL, NULL);
  glfwMakeContextCurrent(window);
  // ��ʼ��gl3w
  gl3wInit();
  // �����¼��ص�
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
  // ��ѯ
  query();
}

void createVertexData() {
  // ����ģ���ļ�
  bool resLoadModel = teapot.loadFromFile(pathModel);
  if (!resLoadModel) {
    std::cout << "��·��" + pathModel + "����ģ��ʧ�ܣ�";
    exit(EXIT_FAILURE);
  }

  bool resLoadPlane = plane.loadFromFile(pathPlane);
  if (!resLoadPlane) {
    std::cout << "��·��" + pathPlane + "����ģ��ʧ�ܣ�";
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
  // ��һ�飬��Ⱦ���������
  renderTeapotToTexture();
}

void drawPlane() {

}

void secondPass() {
  // �ڶ��飬ʹ�õ�һ����Ⱦ�Ĳ��������ʾ��ƽ��ģ���ϡ�
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
  // TODO... �ṩ���棬��ʾ��Ŀ�б��û�����ѡ��ͬ����Ŀ��
  while (true) {
    std::cout << "��ǰ��Ŀ�б����£�" << std::endl;
    std::cout << "1: basic" << std::endl;
    std::cout << "2: blinn phong shading" << std::endl;
    std::cout << "3: textures" << std::endl;
    std::cout << "4: render to texture" << std::endl;
    std::cout << "100: �������̨" << std::endl;

    std::cout << "\n" << std::endl;
    std::cout << "��������Ŀ��ţ�" << std::endl;

    int inputNum = 0;
    std::cin >> inputNum;

    switch (inputNum) {
    case 1: {
      std::cout << "ѡ����1: basic" << std::endl;
      minimizeConsoleWindow();
      ProjectBasicDraw proj;
      proj.run();
      break;
    }

    case 2: {
      std::cout << "ѡ����2: blinn phong shading" << std::endl;
      minimizeConsoleWindow();
      ProjectBlinnPhongShading proj;
      proj.run();
      break;
    }

    case 3: {
      std::cout << "ѡ����3: textures" << std::endl;
      minimizeConsoleWindow();
      // TODO...
      break;
    }

    case 4: {
      std::cout << "ѡ����4: render to texture" << std::endl;
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