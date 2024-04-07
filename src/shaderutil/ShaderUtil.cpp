#include "ShaderUtil.h"
#include <iostream>
#include <fstream>

static std::string readShaderFromFile(const std::string& path) {
  std::string res = "";

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
    ShaderInfo shader = shaders[i];
    GLuint vertShader = glCreateShader(shader.type);
    shader.shaderObj = &vertShader;
    std::string sourceCode = readShaderFromFile(shader.filePath);
    const char* stringSource = sourceCode.c_str();
    // shader ���Դ��
    glShaderSource(vertShader, 1, &stringSource, NULL);
    // ����
    glCompileShader(vertShader);
    // �鿴������
    GLint resShaderCompile = GL_TRUE;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &resShaderCompile);
    if (resShaderCompile == GL_FALSE) {
      // �������ʧ��
      std::cout << shader.type << "shader compile failure!" << std::endl;
      // TODO...д����־
    }
    // ��shader��program��
    glAttachShader(program, vertShader);
  }

  glLinkProgram(program);
  // �鿴���ӽ��
  GLint resProgramLink = GL_TRUE;
  glGetProgramiv(program, GL_COMPILE_STATUS, &resProgramLink);
  if (resProgramLink == GL_FALSE) {
    // ��������ʧ��
    std::cout << "shader program link failure!" << std::endl;
  }

  return program;
}

