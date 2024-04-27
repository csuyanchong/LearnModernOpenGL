#include "GLTextureManager.h"

#include <gl3w/GL/gl3w.h>

namespace {
  std::vector<GLuint> bufferIds;
  GLuint texture_id;
  /* 测试棋盘格像素数据 */
  const GLubyte tex_checkboadr_data[] = {
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
  };

  void setUpCheckBoardImageData(Texture _tex) {

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      8,
      8,
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      tex_checkboadr_data
    );
  }

  void setupTextureBuffer(Texture& _tex, GLuint _index) {
    GLuint& textureId = bufferIds[_index];
    // 创建纹理缓存
    glBindTexture(GL_TEXTURE_2D, textureId);

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
}

void GLTextureManager::setUpTexturesInGPU(std::vector<Texture> _textures) {
  size_t count = _textures.size();
  if (count == 0) {
    // 生成2个1x1像素的贴图, TODO...
    count = 2;
    bufferIds.resize(count);
    glGenTextures((GLsizei)count, bufferIds.data());

    
  }
  else {
    bufferIds.resize(count);
    glGenTextures((GLsizei)count, bufferIds.data());

    // 使用用棋盘格纹理测试
    //setUpCheckBoardImageData(_textures[0]);

    // 查询显卡支持的最大纹理单元数量
    GLint textureCount = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureCount);

    for (size_t i = 0; i < _textures.size(); i++) {
      setupTextureBuffer(_textures[i], (GLuint)i);
    }
  }
}



void GLTextureManager::activeTextures() {
  // 测试激活棋盘格单元
  //glActiveTexture((GLenum)(GL_TEXTURE0));
  //glBindTexture(GL_TEXTURE_2D, texture_id);

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


