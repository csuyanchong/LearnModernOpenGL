#ifndef SRC_PROJECT_SHADING_PROJECTBLINNPHONGSHADING_H_
#define SRC_PROJECT_SHADING_PROJECTBLINNPHONGSHADING_H_

#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "../../model/Model.h"
#include "../../camera/Camera.h"
#include "../../shaderutil/ShaderProgramUtil.h"

class ProjectBlinnPhongShading {
public:
  ProjectBlinnPhongShading();
  ~ProjectBlinnPhongShading();
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
  void passDataToShader(ShaderProgramUtil shaderUtil);

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

  /* ��ǰʹ�õ�shader */
  std::string curVertShader = SHADER_DIR + blinn_phong_vert_shader;
  std::string curFragShader = SHADER_DIR + blinn_phong_frag_shader;

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
};


#endif // !SRC_PROJECT_SHADING_PROJECTBLINNPHONGSHADING_H_