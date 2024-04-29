#ifndef SRC_PROJECT_RENDERBUFFER_PROJECTRENDERTOTEXTURE_H_
#define SRC_PROJECT_RENDERBUFFER_PROJECTRENDERTOTEXTURE_H_

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


class ProjectRenderToTexture {
public:
  ProjectRenderToTexture();
  ~ProjectRenderToTexture();
  void run();

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
  /* 模型文件地址 */
  const std::string MODELS_DIR = "./data/models/objmodel/";
  std::string namePlane = "plane.obj";
  std::string pathPlane = MODELS_DIR + namePlane;

  std::string nameTeapot = "teapot.obj";
  std::string pathTeapot = MODELS_DIR + nameTeapot;

  glm::vec3 colorPlane = glm::vec3(1.0f, 1.0f, 1.0f);

  /* 透视投影 */
  GLfloat FOV = 45.0f;
  GLfloat NEAR_CLIP_PLANE = 0.1f;
  GLfloat FAR_CLIP_PLANE = 100.0f;

  /* 模型变换 */
  GLfloat rotation = 0;
  GLfloat forward = 0;
  GLfloat scale = 0.1f;

  /* 相机 */
  Camera camMain;

  /* 平行光Dirction light光照模型 */
  struct DirectionLight {
    glm::vec3 lightPosition;
    glm::vec3 lightColor;
  };

  DirectionLight light = {
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f)
  };


  /* 图形管线 */
  GLuint shaderProgram;
  
  /* shader文件地址 */
  std::string SHADER_DIR = "./data/shaders/";

  std::string texture_map_vert_shader = "render_to_texture.vert";
  std::string texture_map_frag_shader = "render_to_texture.frag";

  /* 当前使用的shader */
  std::string pathVertShader = SHADER_DIR + texture_map_vert_shader;
  std::string pathFragShader = SHADER_DIR + texture_map_frag_shader;

  /* 计算参数 */
  glm::mat4 modelView;
  glm::mat4 modelViewProjection;
  glm::mat3 modelViewForNormal;

  glm::vec3 dirLight;

  /* 灯光旋转参数 */
  GLfloat lightRotationSpeed = 0;

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
};

#endif // !SRC_PROJECT_RENDERBUFFER_PROJECTRENDERTOTEXTURE_H_

