#ifndef SRC_TEXTURE_TEXTURE_H_
#define SRC_TEXTURE_TEXTURE_H_

#include <string>
#include <vector>

class Texture {
public:
  Texture();
  Texture(std::string _type, std::string _path, unsigned int _width, unsigned int _height, std::vector<unsigned char> _data) :type(_type), path(_path), width(_width), height(_height), data(_data) {};
  void loadFromFile(std::string _path);

  std::string type;
  std::string path;
  unsigned int width;
  unsigned int height;
  std::vector<unsigned char> data;

private:
  std::string directory;
};

#endif // !SRC_TEXTURE_TEXTURE_H_

