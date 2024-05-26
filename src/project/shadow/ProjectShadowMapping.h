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
#include "../../shader/ShaderParam.h"

/// <summary>
/// 相机在灯光位置下的参数。
/// </summary>
struct CameraInLightParam {
  glm::mat4 viewMat;
  glm::mat4 projMat;
};

class ProjectShadowMapping {
public:
  ProjectShadowMapping();
  ~ProjectShadowMapping();
  void run();
  void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
  GLuint createShaderProgram(const std::string& pathVert, const std::string& pathFrag);

  void createWindow();
  void createScene();
  void createRenderPipeline();

  void bindInput();
  void mainLoop();

  CameraInLightParam drawFirstPass();
  void drawSecondPass(const CameraInLightParam& param, const Transform& camTransform);
  void draw();

  void createFrameBuffer();
  GLuint createDepthTexture(GLuint width, GLuint height);
  GLuint createTextureObject(GLuint width, GLuint height);
  GLuint createRenderBufferObject(GLuint width, GLuint height, GLenum type);
  void setClearBuffer1(GLuint frameBufferId, GLfloat* clearColor, GLfloat* clearDepth);
  void computeShaderData();
  DirectionalLightShaderParam computeDirectionalLightShaderData(const glm::mat4 viewMat, const DirectionalLight& light);
  SpotLightShaderParam computeLightShaderData(const glm::mat4 viewMat, const SpotLight& light);
  ModelShaderParam computeModelShaderData(const Transform& trans, const glm::mat4 viewMat, const glm::mat4 projectMat, const glm::vec3 materialColor, GLuint shadowUnitIndex, CameraInLightParam camLightParam);
  SimpleShaderParam computeModelShaderData(const Transform& trans, const glm::mat4 viewMat, const glm::mat4 projectMat);
  void computeShaderData(glm::vec3 pos, GLfloat rotation, GLfloat scale); 
  void passLightDataToShaderProgram(GLuint shaderProgram, const DirectionalLightShaderParam& data);
  void passLightDataToShaderProgram(GLuint shaderProgram, const SpotLightShaderParam& data);
  void passModelDataToShaderProgram(GLuint shaderProgram, const ModelShaderParam& data);
  void passModelDataToShaderProgram(GLuint shaderProgram, const SimpleShaderParam& data);
  void passPlaneDataToShader(GLuint _shaderProgram, glm::mat4 _mvp, glm::vec3 _color);
  void passTeapotDataToShader(GLuint _shaderProgram, glm::mat4 _mvp, glm::vec3 _color);
  void passDataToShader1(GLuint shaderProgram);
  void cleanUp();

private:
  /* 窗口 */
  GLFWwindow* window;
  /* 屏幕宽度 */
  int SCREEN_WIDTH = 1280;
  /* 屏幕高度 */
  int SCREEN_HEIGHT = 720;
  int WINDOW_POSITION_X = 640;
  int WINDOW_POSITION_Y = 200;
  /* 屏幕清除颜色 */
  GLfloat CLEAR_COLOR_BLACK[3] = { 0.0f, 0.0f, 0.0f };
  GLfloat CLEAR_COLOR_GREY[3] = { 0.5f, 0.5f, 0.5f };
  /* 屏幕深度缓存清除值 */
  GLfloat CLEAR_DEPTH = 1.0f;

  Model plane;
  Model teapot;
  Model cube;

  /* 模型文件地址 */
  const std::string MODELS_DIR = "./data/models/objmodel/";
  std::string namePlane = "plane.obj";
  std::string pathPlane = MODELS_DIR + namePlane;

  std::string nameTeapot = "teapot.obj";
  std::string pathTeapot = MODELS_DIR + nameTeapot;

  std::string nameCube = "cube.obj";
  std::string pathCube = MODELS_DIR + nameCube;

  glm::vec3 colorPlane = glm::vec3(0.8f, 0.8f, 0.8f);
  glm::vec3 colorTeapot = glm::vec3(1.0f, 0, 0);

  /* 透视投影 */
  GLfloat FOV = 60.0f;
  GLfloat NEAR_CLIP_PLANE = 0.1f;
  GLfloat FAR_CLIP_PLANE = 100.0f;

  /* 模型变换 */
  GLfloat rotation = 0;
  GLfloat forward = 0;
  GLfloat scale = 0.1f;

  /* 茶壶模型变换信息 */
  Transform teapotTransform;
  glm::vec3 posTeapot = glm::vec3(0, 0, 0);
  GLfloat rotationTeapot = 0;
  GLfloat scaleTeapot = 0.1f;

  /* 平面模型变换信息 */
  Transform planeTransrom;
  glm::vec3 posPlane = glm::vec3(0, 0, 0);
  GLfloat rotationPlane = 0;
  GLfloat scalePlane = 5.0f;
  
  /* 相机变换信息 */
  glm::vec3 posCamera = glm::vec3(0, 2, 3.8f);
  glm::vec3 eulerCamera = glm::vec3(-15, 0, 0);
  glm::vec3 scaleCamera = glm::vec3(1.0f);

  /* 聚光灯变换信息 */
  glm::vec3 posSpotLight = glm::vec3(0, 3, 0);
  glm::vec3 eulerSpotLight = glm::vec3(-90, 0, 0);
  glm::vec3 scaleSpotLight = glm::vec3(1.0f);
  float angleSpotLight = 90.0f;

  /* 相机 */
  Camera camMain;

  /* 聚光灯 */
  SpotLight spotLight;

  /* 直射光*/
  DirectionalLight directionLight;

  /* 图形管线 */
  GLuint shaderProgram;

  GLuint shaderSimpleProgram;

  /* shader文件地址 */
  std::string SHADER_DIR = "./data/shaders/";

  std::string shadow_map_vert_shader = "shadow_map_render.vert";
  std::string shadow_map_frag_shader = "shadow_map_render.frag";

  std::string simple_vert_shader = "simple.vert";
  std::string simple_frag_shader = "simple.frag";

  /* 当前使用的shader */
  std::string pathVertShader = SHADER_DIR + shadow_map_vert_shader;
  std::string pathFragShader = SHADER_DIR + shadow_map_frag_shader;

  std::string pathSimpleVertShader = SHADER_DIR + simple_vert_shader;
  std::string pathSimpleFragShader = SHADER_DIR + simple_frag_shader;

  /* 计算参数 */
  glm::mat4 modelView;
  glm::mat4 modelViewProjection;
  glm::mat3 modelViewForNormal;

  glm::vec3 dirLight;

  /* 灯光旋转参数 */
  GLfloat lightRotationSpeed = 0;

  /* 相机移动参数 */
  GLfloat cameraMoveSpeed = 0.1f;

  /* 纹理id */
  GLuint textureId;
  /* 帧缓存id*/
  GLuint frameBufferId;

  /* 渲染纹理宽度 */
  GLuint textureWidth = 1024;
  /* 渲染纹理高度 */
  GLuint textureHeight = 1024;
  /* 纹理单元*/
  GLuint textureUnitTarget = 100;
  GLuint sampleUnit_1 = 0;

  /* 阴影图纹理采样单元 */
  GLuint shadowTextureUnit = 4;

  GLfloat bias = 0.002f;
};

#endif // !SRC_PROJECT_SHADOW_PROJECTSHADOWMAPPING_H_


