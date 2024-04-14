#include "GLTextureManager.h"

void GLTextureManager::setUpTexturesInGPU(std::vector<Texture> _textures) {
  size_t count = _textures.size();
  if (count == 0) {
    return;
  }

  bufferIds.resize(count);
  glGenTextures((GLsizei)count, bufferIds.data());

  for (size_t i = 0; i < _textures.size(); i++) {
    setupBuffer(_textures[i], (GLuint)i);
  }
}

void GLTextureManager::setupBuffer(Texture& _tex, GLuint _index) {
  GLuint& textureId = bufferIds[_index];
  // 创建纹理缓存
  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexImage2D(
    textureId,
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

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

}

void GLTextureManager::activeTextures() {
  size_t count = bufferIds.size();
  if (count == 0) {
    return;
  }

  for (size_t i = 0; i < count; i++) {
    GLuint textureId = bufferIds[i];
    glActiveTexture((GLenum)(GL_TEXTURE0 + i));
    glBindTexture(GL_TEXTURE_2D, textureId);
  }
}


