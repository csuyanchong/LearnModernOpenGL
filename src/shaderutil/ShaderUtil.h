#ifndef SRC_SHADERUTIL_SHADERUTIL_H_
#define SRC_SHADERUTIL_SHADERUTIL_H_

#include <gl3w/GL/gl3w.h>
#include <string>
#include <vector>

typedef struct {
  const GLenum type;
  const std::string filePath;
  GLuint* shaderObj;
} ShaderInfo;


GLuint loadShader(std::vector<ShaderInfo>& shaders);

#endif // !SRC_SHADERUTIL_SHADERUTIL_H_


