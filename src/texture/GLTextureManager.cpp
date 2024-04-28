#include "GLTextureManager.h"

void GLTextureManager::setUpTexturesInGPU(std::vector<Texture> _textures) {
  size_t count = _textures.size();
  if (count == 0) {
    // ����2��1x1���ص���ͼ��2���Ǹ��ݲ�����������������
    count = 2;
    bufferIds.resize(count);
    glGenTextures((GLsizei)count, bufferIds.data());

    // ʹ���Զ�����������
    setTextureFromDefinedDataRGB(bufferIds[0], 1, 1, tex_1x1);
    setTextureFromDefinedDataRGB(bufferIds[1], 1, 1, tex_1x1);
  }
  else {
    // ʹ��ģ���Դ���������ͼ
    bufferIds.resize(count);
    glGenTextures((GLsizei)count, bufferIds.data());

    for (size_t i = 0; i < _textures.size(); i++) {
      setTextureFromImageDataRGBA(_textures[i], bufferIds[(GLuint)i]);
    }
  }
}

void GLTextureManager::activeTextures() {
  size_t count = bufferIds.size();
  // count ������Ϊ0����Ϊ���û��Texture��Ĭ�ϻ���򴴽�����

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

  // ��������
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  //glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTextureManager::setTextureFromImageDataRGBA(Texture& _tex, GLuint _textureId) {
  // ����������
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

  // ��������
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glGenerateMipmap(GL_TEXTURE_2D);
}