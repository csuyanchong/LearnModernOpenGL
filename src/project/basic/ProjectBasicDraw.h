#ifndef SRC_PROJECT_01BASIC_PROJECTBASICDRAW_H_
#define SRC_PROJECT_01BASIC_PROJECTBASICDRAW_H_

#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "../../model/Model.h"
#include "../../camera/Camera.h"

class ProjectBasicDraw {
public:
  ProjectBasicDraw();
  ~ProjectBasicDraw();
  void run();
private:
  void initSetup();
  void createVertexData();
  void createGraphicPipeline();
  void setCamera();
  void mainLoop();
  void cleanUp();

  void preDraw();
  void draw();

  void setClearFlag();
  void preCompute();
  //void passDataToShader(ShaderProgramUtil shaderUtil);

private:
  /* ���� */
  GLFWwindow* window;
  /* ��Ļ��� */
  int SCREEN_WIDTH = 1280;
  /* ��Ļ�߶� */
  int SCREEN_HEIGHT = 720;
  int SCREEN_POSITION_X = 640;
  int SCREEN_POSITION_Y = 200;
  /* ��Ļ�����ɫ */
  GLfloat CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  /* ��Ļ��Ȼ������ֵ */
  GLfloat CLEAR_DEPTH = 1.0f;

  Model model;
  /* ģ���ļ���ַ */
  const std::string MODELS_DIR = "./data/models/objmodel/";
  std::string nameModel = "teapot.obj";
  std::string pathModel = MODELS_DIR + nameModel;

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
  Camera camMain;

  /* shader�ļ���ַ */

  std::string SHADER_DIR = "./data/shaders/";
  std::string blinn_phong_vert_shader = "blinn_phong.vert";
  std::string blinn_phong_frag_shader = "blinn_phong.frag";

  std::string texture_map_vert_shader = "texture_map.vert";
  std::string texture_map_frag_shader = "texture_map.frag";

  /* ��ǰʹ�õ�shader */
  std::string curVertShader = SHADER_DIR + texture_map_vert_shader;
  std::string curFragShader = SHADER_DIR + texture_map_frag_shader;
};

#endif // !SRC_PROJECT_01BASIC_PROJECTBASICDRAW_H_