#include "Model.h"

/* Standard libray */
#include <iostream>

/* Assimp */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/vec3.hpp>
#include <gl3w/GL/gl3w.h>

/* Src header*/
#include "../mesh/Mesh.h"


#pragma region Helper Function

namespace {
  Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
    // Data to 
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    // Walk through each of the mesh's vertices
    for (GLuint i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex = {};
      vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);     
      vertices.push_back(vertex);
    }

    if (mesh->mNormals != nullptr) {
      for (GLuint i = 0; i < mesh->mNumVertices; i++) {
        Vertex& vertex = vertices[i];
        aiVector3D normal = mesh->mNormals[i];
        vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
      }
    }

    for (GLuint i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];

      for (GLuint j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices);
  }

  void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes) {
    for (unsigned i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene, meshes);
    }
  }
}

#pragma endregion


Model::Model() {
}

Model::~Model() {
}

bool Model::loadFromFile(const std::string& filePath) {
  // 读取并加载模型文件
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate);
  if (nullptr == scene) {
    std::cout << importer.GetErrorString() << std::endl;
    return false;
  }

  // 处理成OpenGL节点
  processNode(scene->mRootNode, scene, meshes);
  return true;
}

void Model::preDraw() {
  for (size_t i = 0; i < meshes.size(); i++) {
    Mesh mesh = meshes[i];
    mesh.preDraw();
  }
}

void Model::draw() {
  for (size_t i = 0; i < meshes.size(); i++) {
    Mesh mesh = meshes[i];
    mesh.draw();
  }
}





