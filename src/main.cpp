// Standard library header
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// Third-party library
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>

// Program src header
#include "shaderutil/ShaderUtil.h"
#include "camera/Camera.h"
#include "mesh/BasicMesh.h"


/* ��Ļ��� */
static const int SCREEN_WIDTH = 1024;
/* ��Ļ�߶� */
static const int SCREEN_HEIGHT = 768;
/* ��Ļ�����ɫ */
static const GLfloat CLEAR_COLOR[] = { 0.0f, 0.0f, 0.0f, 0.0f };
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

/* �任 */
GLfloat rotation = 0;
GLfloat forward = -2.0f;
GLfloat scale = 1.0f;

/* ������� */
Camera camMain;

GLfloat moveCamVerticalSpeed = 0.1f;
GLfloat moveCamHorizenSpeed = 0.1f;

/* ģ���ļ���ַ */
const std::string MODELS_DIR = "./data/models/objmodel/";
std::string nameModel = "teapot.obj";

std::string pathModel = MODELS_DIR + nameModel;

void preDraw() {
  glClearBufferfv(GL_COLOR, 0, CLEAR_COLOR);

  // ʹ��ͼ�ι���
  glUseProgram(programPipeline);

  glm::mat4 model = glm::mat4(1.0f);

  model = glm::translate(model, glm::vec3(0, 0, forward));

  model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1.0f, 0));

  model = glm::scale(model, glm::vec3(scale));


  glm::mat4 view = camMain.getViewMatrix();

  glm::mat4 project = glm::perspective(FOV, (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

  glm::mat4 mvp = project * view * model;

  // ��ѯ���޸�ȫ�ֱ���
  GLint location_uniform_projMat = glGetUniformLocation(programPipeline, "uMVP");
  if (location_uniform_projMat != -1) {
    glUniformMatrix4fv(location_uniform_projMat, 1, GL_FALSE, &mvp[0][0]);
  }
  else {
    std::cout << "��ѯȫ�ֱ���projMatû�ҵ�������ƴд����" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void draw() {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glDrawElements(GL_TRIANGLES, (GLsizei)verticesIndex.size(), GL_UNSIGNED_INT, NULL);
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
    scale += 0.1f;
    scale = fmin(scale, 2.0f);
  }
  if (key == GLFW_KEY_G) {
    scale -= 0.1f;
    scale = fmaxf(scale, 0.2f);
  }
  if (key == GLFW_KEY_N)
  {
    scale = 1;
  }
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void initSetup() {
  // ��ʼ��glfw
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "glfw library ��ʼ��ʧ�ܣ�" << std::endl;
    exit(EXIT_FAILURE);
  }
  // ��������
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnMordenOpenGL", NULL, NULL);
  glfwMakeContextCurrent(window);
  // ��ʼ��gl3w
  gl3wInit();
  // �����¼��ص�
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
}

void createVertexData() {
  // ����vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // ����ģ���ļ�
  BasicMesh mesh;
  bool res = mesh.loadMeshFromFile(pathModel);
  if (!res) {
    std::cout << "��·��" + pathModel + "����ģ��ʧ�ܣ�";
    exit(EXIT_FAILURE);
  }

  // ָ����������
  verticesPosition = {
    -0.5f, 0.5f,
    -0.5f, -0.5f,
    0.5f, -0.5f,
    0.5f, 0.5f
  };

  verticesColor = {
    255, 0, 0, // ��ɫ
    0, 255, 0, // ��ɫ
    0, 0, 255 // ��ɫ
  };

  verticesIndex = {
    0, 1, 2, 0, 2, 3
  };

  // ����λ�û���
  glGenBuffers((GLsizei)vbo.size(), vbo.data());
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, verticesPosition.size() * sizeof(GLfloat), verticesPosition.data(), GL_STATIC_DRAW);

  // ��vao����attribute��Ӧ�Ļ�������
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glDisableVertexAttribArray(0);

  // ������ɫ����
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, verticesColor.size() * sizeof(GLubyte), verticesColor.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
  glDisableVertexAttribArray(1);

  // ��������
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesIndex.size() * sizeof(GLuint), verticesIndex.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(2, 3, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
  glDisableVertexAttribArray(2);
}

void createGraphicPipeline() {
  std::vector<ShaderInfo> shaders;
  ShaderInfo vertInfo = {
    GL_VERTEX_SHADER, "./data/shaders/vert.glsl"
  };

  ShaderInfo fragInfo = {
    GL_FRAGMENT_SHADER, "./data/shaders/frag.glsl"
  };

  shaders.push_back(vertInfo);
  shaders.push_back(fragInfo);

  programPipeline = loadShader(shaders);
}

void mainLoop() {
  while (!glfwWindowShouldClose(window))
  {
    preDraw();
    draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void cleanUp() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

int main(int argc, char* argv[]) {
  // 1.��ʼ������
  initSetup();
  // 2.������������
  createVertexData();
  // 3.����ͼ�ι���
  createGraphicPipeline();
  // 4.��ѭ��
  mainLoop();
  // 5.����
  cleanUp();
  return 0;
}