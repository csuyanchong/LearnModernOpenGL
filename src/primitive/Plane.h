#ifndef SRC_PRIMITIVE_PLANE_H_
#define SRC_PRIMITIVE_PLANE_H_

#include "../sceneobject/SceneObject.h"

#include <vector>
#include "../mesh/Mesh.h"

class Plane : public SceneObject {
public:
  Plane();
  void preDraw();
  void draw();

protected:

private:
  std::vector<Mesh> meshes;
};

#endif // !SRC_PRIMITIVE_PLANE_H_


