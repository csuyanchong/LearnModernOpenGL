#ifndef SRC_PROJECT_SHADOW_PROJECTSHADOWMAPPING_H_
#define SRC_PROJECT_SHADOW_PROJECTSHADOWMAPPING_H_

// Standard library
#include <string>

// Third party
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

// Program defined
#include "../../model/Model.h"
#include "../../camera/Camera.h"
#include "../../light/Light.h"

class ProjectShadowMapping {
public:
  ProjectShadowMapping();
  ~ProjectShadowMapping();
  void run();
  void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
  void createWindow();
  void createScene();
  void createRenderPipeline();
  void bindInput();
  void mainLoop();
  void draw();
  void drawFirstPass();
  void createFrameBuffer();
  GLuint createTextureObject(GLuint width, GLuint height);
  GLuint createRenderBufferObject(GLuint width, GLuint height, GLenum type);
  void drawTarget(GLuint frameBufferId);
  void passDataToShader2(GLuint shaderProgram);
  void drawSecondPass();
  void setClearBuffer1(GLuint frameBufferId, GLfloat* clearColor, GLfloat* clearDepth);
  void computeShaderData();
  void computeShaderData(glm::vec3 pos, GLfloat rotation, GLfloat scale);
  void passPlaneDataToShader(GLuint _shaderProgram, glm::mat4 _mvp, glm::vec3 _color);
  void passTeapotDataToShader(GLuint _shaderProgram, glm::mat4 _mvp, glm::vec3 _color);
  void passDataToShader1(GLuint shaderProgram);
  void cleanUp();

private:
  /* ���� */
  GLFWwindow* window;
  /* ��Ļ��� */
  int SCREEN_WIDTH = 1280;
  /* ��Ļ�߶� */
  int SCREEN_HEIGHT = 720;
  int WINDOW_POSITION_X = 640;
  int WINDOW_POSITION_Y = 200;
  /* ��Ļ�����ɫ */
  GLfloat CLEAR_COLOR_BLACK[3] = { 0.0f, 0.0f, 0.0f };
  GLfloat CLEAR_COLOR_GREY[3] = { 0.5f, 0.5f, 0.5f };
  /* ��Ļ��Ȼ������ֵ */
  GLfloat CLEAR_DEPTH = 1.0f;

  Model plane;
  Model teapot;
  Model cube;

  /* ģ���ļ���ַ */
  const std::string MODELS_DIR = "./data/models/objmodel/";
  std::string namePlane = "plane.obj";
  std::string pathPlane = MODELS_DIR + namePlane;

  std::string nameTeapot = "teapot.obj";
  std::string pathTeapot = MODELS_DIR + nameTeapot;

  std::string nameCube = "cube.obj";
  std::string pathCube = MODELS_DIR + nameCube;

  glm::vec3 colorPlane = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::vec3 colorTeapot = glm::vec3(1.0f, 0, 0);

  /* ͸��ͶӰ */
  GLfloat FOV = 45.0f;
  GLfloat NEAR_CLIP_PLANE = 0.1f;
  GLfloat FAR_CLIP_PLANE = 100.0f;

  /* ģ�ͱ任 */
  GLfloat rotation = 0;
  GLfloat forward = 0;
  GLfloat scale = 0.1f;

  /* ���ģ�ͱ任��Ϣ */
  glm::vec3 posTeapot = glm::vec3(0, 1.0f, 0);
  GLfloat rotationTeapot = 135;
  GLfloat scaleTeapot = 0.06f;

  /* ƽ��ģ�ͱ任��Ϣ */
  glm::vec3 posPlane = glm::vec3(0, 0, 0);
  GLfloat rotationPlane = 0;
  GLfloat scalePlane = 5.0f;
  
  /* ��� */
  Camera camMain;

  /* �۹�� */
  SpotLight light;

  /* ͼ�ι��� */
  GLuint shaderProgram;

  /* shader�ļ���ַ */
  std::string SHADER_DIR = "./data/shaders/";

  std::string shadow_map_vert_shader = "shadow_map_render.vert";
  std::string shadow_map_frag_shader = "shadow_map_render.frag";

  /* ��ǰʹ�õ�shader */
  std::string pathVertShader = SHADER_DIR + shadow_map_vert_shader;
  std::string pathFragShader = SHADER_DIR + shadow_map_frag_shader;

  /* ������� */
  glm::mat4 modelView;
  glm::mat4 modelViewProjection;
  glm::mat3 modelViewForNormal;

  glm::vec3 dirLight;

  /* �ƹ���ת���� */
  GLfloat lightRotationSpeed = 0;

  /* ����id */
  GLuint textureId;
  /* ֡����id*/
  GLuint frameBufferId;

  /* ��Ⱦ������ */
  GLuint textureWidth = 1024;
  /* ��Ⱦ����߶� */
  GLuint textureHeight = 1024;
  /* ����Ԫ*/
  GLuint textureUnitTarget = 100;
  GLuint sampleUnit_1 = 0;
};

#endif // !SRC_PROJECT_SHADOW_PROJECTSHADOWMAPPING_H_


