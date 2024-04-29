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
  /* ģ���ļ���ַ */
  const std::string MODELS_DIR = "./data/models/objmodel/";
  std::string namePlane = "plane.obj";
  std::string pathPlane = MODELS_DIR + namePlane;

  std::string nameTeapot = "teapot.obj";
  std::string pathTeapot = MODELS_DIR + nameTeapot;

  glm::vec3 colorPlane = glm::vec3(1.0f, 1.0f, 1.0f);

  /* ͸��ͶӰ */
  GLfloat FOV = 45.0f;
  GLfloat NEAR_CLIP_PLANE = 0.1f;
  GLfloat FAR_CLIP_PLANE = 100.0f;

  /* ģ�ͱ任 */
  GLfloat rotation = 0;
  GLfloat forward = 0;
  GLfloat scale = 0.1f;

  /* ��� */
  Camera camMain;

  /* ƽ�й�Dirction light����ģ�� */
  struct DirectionLight {
    glm::vec3 lightPosition;
    glm::vec3 lightColor;
  };

  DirectionLight light = {
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f)
  };


  /* ͼ�ι��� */
  GLuint shaderProgram;
  
  /* shader�ļ���ַ */
  std::string SHADER_DIR = "./data/shaders/";

  std::string texture_map_vert_shader = "render_to_texture.vert";
  std::string texture_map_frag_shader = "render_to_texture.frag";

  /* ��ǰʹ�õ�shader */
  std::string pathVertShader = SHADER_DIR + texture_map_vert_shader;
  std::string pathFragShader = SHADER_DIR + texture_map_frag_shader;

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

#endif // !SRC_PROJECT_RENDERBUFFER_PROJECTRENDERTOTEXTURE_H_

