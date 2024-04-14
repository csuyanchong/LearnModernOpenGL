#include "Model.h"

/* Standard libray */
#include <iostream>

/* Assimp */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/vec3.hpp>
#include <gl3w/GL/gl3w.h>

#include <lodepng/lodepng.h>

/* Src header*/
#include "../mesh/Mesh.h"

#pragma region Helper Function

namespace {
  std::vector<Texture> _textures;
  std::vector<Texture> _textures_loaded;
  std::string _directory;

  /* 辅助函数声明 */
  void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
  void loadEmbeddedTexture(const aiTexture* embeddedTexture, Texture& _texture);


  /* 辅助函数功能实现 */
  void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes) {
    for (unsigned i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene, meshes);
    }

  }

  Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
    // Data to 
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    // Walk through each of the mesh's vertices
    for (GLuint i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex = {};
      vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

      if (mesh->mTextureCoords[0]) {
        vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
        vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
      }

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

    if (mesh->mMaterialIndex >= 0) {
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

      std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
      // TODO...处理其它类型的纹理，比如高光、法线贴图等。
      _textures.insert(_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    }

    return Mesh(vertices, indices);
  }

  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene) {
    std::vector<Texture> textures;
    for (UINT i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);
      // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
      bool skip = false;
      for (UINT j = 0; j < _textures_loaded.size(); j++) {
        if (std::strcmp(_textures_loaded[j].path.c_str(), str.C_Str()) == 0) {
          textures.push_back(_textures_loaded[j]);
          skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
          break;
        }
      }
      if (!skip) {   // If texture hasn't been loaded already, load it
        Texture texture;

        const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
        if (embeddedTexture != nullptr) {
          loadEmbeddedTexture(embeddedTexture, texture);
        }
        else {
          std::string filename = std::string(str.C_Str());
          filename = _directory + '/' + filename;
          texture.loadFromFile(filename);
          /*std::wstring filenamews = std::wstring(filename.begin(), filename.end());
          hr = CreateWICTextureFromFile(dev_, devcon_, filenamews.c_str(), nullptr, &texture.texture);
          if (FAILED(hr))
            MessageBox(hwnd_, "Texture couldn't be loaded", "Error!", MB_ICONERROR | MB_OK);*/
        }
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
        _textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
      }
    }
    return textures;
  }

  void loadEmbeddedTexture(const aiTexture* embeddedTexture, Texture& _texture) {
    _texture.width = embeddedTexture->mWidth;
    _texture.height = embeddedTexture->mHeight;

    std::vector<unsigned char> _data;
    aiTexel* pcData = embeddedTexture->pcData;

    // 转码成RGBA
    while (pcData != nullptr) {
      _data.push_back(pcData->r);
      _data.push_back(pcData->g);
      _data.push_back(pcData->b);
      _data.push_back(pcData->a);
      pcData++;
    }
    _texture.data = _data;
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

  _directory = filePath.substr(0, filePath.find_last_of("/\\"));

  // 处理OpenGL节点
  processNode(scene->mRootNode, scene, meshes);

  // 获取处理过的纹理集合
  textures = _textures;

  // 创建纹理缓冲
  textureManager.setUpTexturesInGPU(textures);

  return true;
}

void Model::preDraw() {
  for (size_t i = 0; i < meshes.size(); i++) {
    Mesh mesh = meshes[i];
    mesh.preDraw();
  }
  textureManager.activeTextures();
}

void Model::draw() {
  for (size_t i = 0; i < meshes.size(); i++) {
    Mesh mesh = meshes[i];
    mesh.draw();
  }
}





