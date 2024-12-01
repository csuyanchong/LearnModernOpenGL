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
    // ���ļ�ʧ��
    std::cout << "���ļ�" + path + "ʧ�ܣ�" << std::endl;
  }
  return res;
}

GLuint loadShader(std::vector<ShaderInfo>& shaders) {
  GLuint program = glCreateProgram();

  // ��������shader
  for (size_t i = 0; i < shaders.size(); i++) {
    ShaderInfo shaderInfo = shaders[i];
    GLuint shader = glCreateShader(shaderInfo.type);
    shaderInfo.shaderObj = &shader;
    std::string sourceCode = readShaderFromFile(shaderInfo.filePath);
    const char* stringSource = sourceCode.c_str();
    // shader ���Դ��
    glShaderSource(shader, 1, &stringSource, nullptr);
    // ����
    glCompileShader(shader);
    // �鿴������
    GLint resShaderCompile = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &resShaderCompile);
    if (resShaderCompile == GL_FALSE) {
      // �������ʧ��
      std::cout << "λ��" + shaderInfo.filePath + "����shader����ʧ��!" << std::endl;
      GLsizei log_length = 0;
      GLchar message[1024];
      glGetShaderInfoLog(shader, 1024, &log_length, message);
      std::cout << message << std::endl;
      exit(EXIT_FAILURE);
      // TODO...д����־
    }
    // ��shader��program��
    glAttachShader(program, shader);
  }

  glLinkProgram(program);
  // �鿴���ӽ��
  GLint resProgramLink = GL_TRUE;
  glGetProgramiv(program, GL_COMPILE_STATUS, &resProgramLink);
  if (resProgramLink == GL_FALSE) {
    // ��������ʧ��
    std::cout << "shader program link failure!" << std::endl;
    GLsizei log_length = 0;
    GLchar message[1024];
    glGetProgramInfoLog(program, 1024, &log_length, message);
    std::cout << message << std::endl;
    exit(EXIT_FAILURE);
  }

  return program;
}

