#ifndef SRC_TEXTURE_GKTEXTUREUTIL_H_
#define SRC_TEXTURE_GKTEXTUREUTIL_H_

// stl
#include <vector>

// plugin
#include <gl3w/GL/gl3w.h>

// program
#include "Texture.h"

class GLTextureManager {
public:
  void setUpTexturesInGPU(std::vector<Texture> _textures);
  void activeTextures();
  std::vector<GLuint> getTextureBuffers() const;

private:
  void setTextureFromDefinedDataRGB(GLuint idTexture, GLuint width, GLuint height, GLubyte* data);
  void setTextureFromImageDataRGBA(Texture& _tex, GLuint _textureId);

private:
  std::vector<GLuint> bufferIds;
  /* 1x1ÏñËØ°×É«255, 255, 255 */
  GLubyte tex_1x1[3] = {
    255, 255, 255
  };

};

#endif // !SRC_TEXTURE_GKTEXTUREUTIL_H_

