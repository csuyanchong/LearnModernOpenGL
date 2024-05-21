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
#include <glm/trigonometric.hpp>

// Program defined
#include "../../model/Model.h"
#include "../../camera/Camera.h"
#include "../../light/Light.h"
#include "../../transform/Transform.h"


/// <summary>
/// �ƹ�shader���������
/// </summary>
class LightShaderParam {
public:
  glm::vec3 position;
  glm::vec3 toward;
  GLfloat angle;
};

class DirectionalLightShaderParam {
public:
  glm::vec3 direction;
};

class SpotLightShaderParam {
public:
  glm::vec3 position;
  glm::vec3 toward;
  GLfloat angle;
};

/// <summary>
/// ģ��shader���������
/// </summary>
class ModelShaderParam {
public:
  glm::mat4 mvp;
  glm::mat4 mv;
  glm::mat3 mvNormal;
  glm::vec3 color;
};

class ProjectShadowMapping {
public:
  ProjectShadowMapping();
  ~ProjectShadowMapping();
  void run();
  void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
  void createWindow();
  void createScene();
  void createRenderPipeline();
  void bindInput();
  void mainLoop();
  void draw();
  void drawFirstPass();
  void createFrameBuffer();
  GLuint createDepthTexture(GLuint width, GLuint height);
  GLuint createTextureObject(GLuint width, GLuint height);
  GLuint createRenderBufferObject(GLuint width, GLuint height, GLenum type);
  void drawTarget(GLuint frameBufferId);
  void passDataToShader2(GLuint shaderProgram);
  void drawSecondPass();
  void setClearBuffer1(GLuint frameBufferId, GLfloat* clearColor, GLfloat* clearDepth);
  void computeShaderData();
  DirectionalLightShaderParam computeDirectionalLightShaderData(const glm::mat4 viewMat, const DirectionalLight& light);
  SpotLightShaderParam computeLightShaderData(const glm::mat4 viewMat, const SpotLight& light);
  ModelShaderParam computeModelShaderData(const Transform& trans, const glm::mat4 viewMat, const glm::mat4 projectMat, const glm::vec3 materialColor);
  void computeShaderData(glm::vec3 pos, GLfloat rotation, GLfloat scale); 
  void passLightDataToShaderProgram(GLuint shaderProgram, const DirectionalLightShaderParam& data);
  void passLightDataToShaderProgram(GLuint shaderProgram, const SpotLightShaderParam& data);
  void passModelDataToShaderProgram(GLuint shaderProgram, const ModelShaderParam& data);
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

  glm::vec3 colorPlane = glm::vec3(0.8f, 0.8f, 0.8f);
  glm::vec3 colorTeapot = glm::vec3(1.0f, 0, 0);

  /* ͸��ͶӰ */
  GLfloat FOV = 60.0f;
  GLfloat NEAR_CLIP_PLANE = 0.1f;
  GLfloat FAR_CLIP_PLANE = 100.0f;

  /* ģ�ͱ任 */
  GLfloat rotation = 0;
  GLfloat forward = 0;
  GLfloat scale = 0.1f;

  /* ���ģ�ͱ任��Ϣ */
  Transform teapotTransform;
  glm::vec3 posTeapot = glm::vec3(0, 0, 0);
  GLfloat rotationTeapot = 0;
  GLfloat scaleTeapot = 0.1f;

  /* ƽ��ģ�ͱ任��Ϣ */
  Transform planeTransrom;
  glm::vec3 posPlane = glm::vec3(0, 0, 0);
  GLfloat rotationPlane = 0;
  GLfloat scalePlane = 5.0f;
  
  /* ����任��Ϣ */
  glm::vec3 posCamera = glm::vec3(0, 2, 3.8f);
  glm::vec3 eulerCamera = glm::vec3(-15, 0, 0);
  glm::vec3 scaleCamera = glm::vec3(1.0f);

  /* �۹�Ʊ任��Ϣ */
  glm::vec3 posSpotLight = glm::vec3(0, 3, 0);
  glm::vec3 eulerSpotLight = glm::vec3(-90, 0, 0);
  glm::vec3 scaleSpotLight = glm::vec3(1.0f);
  float angleSpotLight = 90.0f;

  /* ��� */
  Camera camMain;

  /* �۹�� */
  SpotLight spotLight;

  /* ֱ���*/
  DirectionalLight directionLight;

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

  /* ����ƶ����� */
  GLfloat cameraMoveSpeed = 0.1f;

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


