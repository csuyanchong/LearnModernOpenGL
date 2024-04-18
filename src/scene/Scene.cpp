#include "Scene.h"

#include <iostream>

#include "../primitive/Plane.h"
#include "../model/Model.h"

Scene::Scene() {
  /* 模型文件地址 */
  const std::string MODELS_DIR = "./data/models/objmodel/";
  std::string nameModel = "teapot.obj";
  std::string pathModel = MODELS_DIR + nameModel;

  Plane plane = Plane();

  Model teapot = Model();
  // 加载模型文件
  bool resLoadModel = teapot.loadFromFile(pathModel);
  if (!resLoadModel) {
    std::cout << "从路径" + pathModel + "加载模型失败！";
    exit(EXIT_FAILURE);
  }

  world.push_back(plane);
  world.push_back(teapot);
}
