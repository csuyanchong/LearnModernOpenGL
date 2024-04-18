#include "Scene.h"

#include <iostream>

#include "../primitive/Plane.h"
#include "../model/Model.h"

Scene::Scene() {
  /* ģ���ļ���ַ */
  const std::string MODELS_DIR = "./data/models/objmodel/";
  std::string nameModel = "teapot.obj";
  std::string pathModel = MODELS_DIR + nameModel;

  Plane plane = Plane();

  Model teapot = Model();
  // ����ģ���ļ�
  bool resLoadModel = teapot.loadFromFile(pathModel);
  if (!resLoadModel) {
    std::cout << "��·��" + pathModel + "����ģ��ʧ�ܣ�";
    exit(EXIT_FAILURE);
  }

  world.push_back(plane);
  world.push_back(teapot);
}
