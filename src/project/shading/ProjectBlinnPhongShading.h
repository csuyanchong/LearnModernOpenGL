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
  /* 窗口 */
  GLFWwindow* window;
  /* 屏幕宽度 */
  int SCREEN_WIDTH = 1280;
  /* 屏幕高度 */
  int SCREEN_HEIGHT = 720;
  int WINDOW_POSITION_X = 640;
  int WINDOW_POSITION_Y = 200;
  /* 屏幕清除颜色 */
  GLfloat CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  /* 屏幕深度缓存清除值 */
  GLfloat CLEAR_DEPTH = 1.0f;

  Model model;
  /* 模型文件地址 */
  const std::string MODELS_DIR = "./data/models/objmodel/";
  std::string nameModel = "teapot.obj";
  std::string pathModel = MODELS_DIR + nameModel;

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
  Camera camMain;

  /* shader文件地址 */
  std::string SHADER_DIR = "./data/shaders/";
  std::string blinn_phong_vert_shader = "blinn_phong.vert";
  std::string blinn_phong_frag_shader = "blinn_phong.frag";

  /* 当前使用的shader */
  std::string curVertShader = SHADER_DIR + blinn_phong_vert_shader;
  std::string curFragShader = SHADER_DIR + blinn_phong_frag_shader;

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
};


#endif // !SRC_PROJECT_SHADING_PROJECTBLINNPHONGSHADING_H_