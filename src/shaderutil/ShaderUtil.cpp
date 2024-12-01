#include "ShaderUtil.h"
#include <iostream>
#include <fstream>

static std::string readShaderFromFile(const std::string& path) {
  std::string res;

  std::ifstream file;
  file.open(path);

  if (file) {
    std::string line;
    while (std::getline(file, line)) {
      res.append(line + "\n");
    }

    file.close();
  }
  else {
    // 打开文件失败
    std::cout << "打开文件" + path + "失败！" << std::endl;
  }
  return res;
}

GLuint loadShader(std::vector<ShaderInfo>& shaders) {
  GLuint program = glCreateProgram();

  // 编译链接shader
  for (size_t i = 0; i < shaders.size(); i++) {
    ShaderInfo shaderInfo = shaders[i];
    GLuint shader = glCreateShader(shaderInfo.type);
    shaderInfo.shaderObj = &shader;
    std::string sourceCode = readShaderFromFile(shaderInfo.filePath);
    const char* stringSource = sourceCode.c_str();
    // shader 填充源码
    glShaderSource(shader, 1, &stringSource, nullptr);
    // 编译
    glCompileShader(shader);
    // 查看编译结果
    GLint resShaderCompile = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &resShaderCompile);
    if (resShaderCompile == GL_FALSE) {
      // 处理编译失败
      std::cout << "位于" + shaderInfo.filePath + "处的shader编译失败!" << std::endl;
      GLsizei log_length = 0;
      GLchar message[1024];
      glGetShaderInfoLog(shader, 1024, &log_length, message);
      std::cout << message << std::endl;
      exit(EXIT_FAILURE);
      // TODO...写入日志
    }
    // 将shader和program绑定
    glAttachShader(program, shader);
  }

  glLinkProgram(program);
  // 查看链接结果
  GLint resProgramLink = GL_TRUE;
  glGetProgramiv(program, GL_COMPILE_STATUS, &resProgramLink);
  if (resProgramLink == GL_FALSE) {
    // 处理链接失败
    std::cout << "shader program link failure!" << std::endl;
    GLsizei log_length = 0;
    GLchar message[1024];
    glGetProgramInfoLog(program, 1024, &log_length, message);
    std::cout << message << std::endl;
    exit(EXIT_FAILURE);
  }

  return program;
}

