#ifndef SRC_SCENE_SCENE_H_
#define SRC_SCENE_SCENE_H_

#include <vector>

#include "../sceneobject/SceneObject.h"

class Scene {
public:
  Scene();
  
public:
  std::vector<SceneObject> world;
};


#endif // !SRC_SCENE_SCENE_H_
