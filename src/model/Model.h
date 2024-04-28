#ifndef SRC_MODEL_MODEL_H_
#define SRC_MODEL_MODEL_H_

// stl
#include <string>
#include <vector>

// plugin 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// program
#include "../mesh/Mesh.h"
#include "../texture/Texture.h"
#include "../texture/GLTextureManager.h"

class Model {
public:
  Model();
  ~Model();
  bool loadFromFile(const std::string& filePath);
  void draw();
  
public:
  std::vector<Mesh> meshes;
  std::vector<Texture> textures;
  GLTextureManager textureManager;

private:
  void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
  void loadEmbeddedTexture(const aiTexture* embeddedTexture, Texture& _texture);

private:
  std::vector<Texture> _textures;
  std::vector<Texture> _textures_loaded;
  std::string _directory;
};

#endif // !SRC_MODEL_MODEL_H_



