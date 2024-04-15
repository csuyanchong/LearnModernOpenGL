#ifndef SRC_TEXTURE_GKTEXTUREUTIL_H_
#define SRC_TEXTURE_GKTEXTUREUTIL_H_

#include <vector>

#include "Texture.h"

class GLTextureManager {
public:
  void setUpTexturesInGPU(std::vector<Texture> _textures);
  void activeTextures();
};

#endif // !SRC_TEXTURE_GKTEXTUREUTIL_H_

