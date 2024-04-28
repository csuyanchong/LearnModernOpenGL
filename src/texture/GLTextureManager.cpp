#include "GLTextureManager.h"

void GLTextureManager::setUpTexturesInGPU(std::vector<Texture> _textures) {
  size_t count = _textures.size();
  if (count == 0) {
    // 生成2个1x1像素的贴图，2个是根据采样器的数量来定的
    count = 2;
    bufferIds.resize(count);
    glGenTextures((GLsizei)count, bufferIds.data());

    // 使用自定义像素数据
    setTextureFromDefinedDataRGB(bufferIds[0], 1, 1, tex_1x1);
    setTextureFromDefinedDataRGB(bufferIds[1], 1, 1, tex_1x1);
  }
  else {
    // 使用模型自带的纹理贴图
    bufferIds.resize(count);
    glGenTextures((GLsizei)count, bufferIds.data());

    for (size_t i = 0; i < _textures.size(); i++) {
      setTextureFromImageDataRGBA(_textures[i], bufferIds[(GLuint)i]);
    }
  }
}

void GLTextureManager::activeTextures() {
  size_t count = bufferIds.size();
  // count 不可能为0，因为如果没有Texture，默认会程序创建纹理。

  for (size_t i = 0; i < count; i++) {
    GLuint textureId = bufferIds[i];
    glActiveTexture((GLenum)(GL_TEXTURE0 + i));
    glBindTexture(GL_TEXTURE_2D, textureId);
  }
}

std::vector<GLuint> GLTextureManager::getTextureBuffers() const {
  return bufferIds;
}

void GLTextureManager::setTextureFromDefinedDataRGB(GLuint idTexture, GLuint width, GLuint height, GLubyte* data) {
  glBindTexture(GL_TEXTURE_2D, idTexture);

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    width,
    height,
    0,
    GL_RGB,
    GL_UNSIGNED_BYTE,
    data
  );

  // 参数设置
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  //glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTextureManager::setTextureFromImageDataRGBA(Texture& _tex, GLuint _textureId) {
  // 创建纹理缓存
  glBindTexture(GL_TEXTURE_2D, _textureId);

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    _tex.width,
    _tex.height,
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    _tex.data.data()
  );

  // 参数设置
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glGenerateMipmap(GL_TEXTURE_2D);
}