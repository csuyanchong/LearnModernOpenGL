#ifndef SRC_TEXTURE_GKTEXTUREUTIL_H_
#define SRC_TEXTURE_GKTEXTUREUTIL_H_

#include <vector>
#include <gl3w/GL/gl3w.h>

#include "Texture.h"

class GLTextureManager {
public:
  void setUpTexturesInGPU(std::vector<Texture> _textures);
  void activeTextures();
private:
  void setupBuffer(Texture& _tex, GLuint _index);
private:
  std::vector<GLuint> bufferIds;
};

#endif // !SRC_TEXTURE_GKTEXTUREUTIL_H_

