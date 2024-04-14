#include "Texture.h"

#include <gl3w/GL/gl3w.h>
#include <lodepng/lodepng.h>
#include <iostream>

namespace {
  GLuint textureId;

  void loadPng(std::string filePath, Texture& _texture) {
    // Load file and decode image.
    unsigned error = lodepng::decode(_texture.data, _texture.width, _texture.height, filePath);

    // If there's an error, display it.
    if (error != 0) {
      std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

Texture::Texture() {
  type = "";
  path = "";
  width = 0;
  height = 0;
  data = {};
}

void Texture::loadFromFile(std::string _path) {
  // 解码png
  loadPng(_path, *this);

  // TODO... 解析其它格式，如jpg。
}

