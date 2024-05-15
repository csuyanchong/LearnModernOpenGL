#ifndef SRC_SHADERUTIL_SHADER_PROGRAM_UTIL_H_
#define SRC_SHADERUTIL_SHADER_PROGRAM_UTIL_H_

#include <string>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <gl3w/GL/gl3w.h>

class ShaderProgramUtil {
public:
  ShaderProgramUtil(GLuint _programPipeline) :programPipeline(_programPipeline) {};

  bool glModifyUniformMat44(const std::string&, glm::mat4);
  bool glModifyUniformMat33(const std::string&, glm::mat3);
  bool glModifyUniformVec3(const std::string& g, glm::vec3);
  bool glModifyUniformFloat(const std::string&, GLfloat);
  bool glModifyUniformInt1(const std::string&, GLint);

private:
  GLuint programPipeline;
};

#endif // !SRC_SHADERUTIL_SHADER_PROGRAM_UTIL_H_



