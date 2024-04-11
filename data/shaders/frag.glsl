#version 450 core

//in vec4 fColor;

layout (location = 0) out vec4 color;

void main() {
//  color = fColor;
  color = vec4(1.0f, 1.0f, 0, 1.0f);
}