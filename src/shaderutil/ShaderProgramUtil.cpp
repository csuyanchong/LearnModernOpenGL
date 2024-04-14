#include "ShaderProgramUtil.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

bool ShaderProgramUtil::glModifyUniformMat44(std::string id, glm::mat4 value) {
  // 查询并修改全局变量
  GLint location = glGetUniformLocation(programPipeline, id.c_str());
  if (location != -1) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    return true;
  }
  std::cout << "查询全局变量" + id + "没找到，可能拼写错误！" << std::endl;
  return false;
}

bool ShaderProgramUtil::glModifyUniformMat33(std::string id, glm::mat3 value) {
  // 查询并修改全局变量
  GLint location = glGetUniformLocation(programPipeline, id.c_str());
  if (location != -1) {
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    return true;
  }
  std::cout << "查询全局变量" + id + "没找到，可能拼写错误！" << std::endl;
  return false;
}

bool ShaderProgramUtil::glModifyUniformVec3(std::string id, glm::vec3 value) {
  // 查询并修改全局变量
  GLint location = glGetUniformLocation(programPipeline, id.c_str());
  if (location != -1) {
    glUniform3fv(location, 1, glm::value_ptr(value));
    return true;
  }
  std::cout << "查询全局变量" + id + "没找到，可能拼写错误！" << std::endl;
  return false;
}

bool ShaderProgramUtil::glModifyUniformFloat(std::string id, GLfloat value) {
  // 查询并修改全局变量
  GLint location = glGetUniformLocation(programPipeline, id.c_str());
  if (location != -1) {
    glUniform1f(location, value);
    return true;
  }
  std::cout << "查询全局变量" + id + "没找到，可能拼写错误！" << std::endl;
  return false;
}

bool ShaderProgramUtil::glModifyUniformInt1(std::string id, GLint value) {
  // 查询并修改全局变量
  GLint location = glGetUniformLocation(programPipeline, id.c_str());
  if (location != -1) {
    glUniform1i(location, value);
    return true;
  }
  std::cout << "查询全局变量" + id + "没找到，可能拼写错误！" << std::endl;
  return false;
}
