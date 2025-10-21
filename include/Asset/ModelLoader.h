#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "../Components/mesh.h"
#include "shader.h"
#include "texture.h"
#include "../Engine/GameObject.h"

#include <string>
#include <fstream>
#include <vector>

namespace Asset {
class ModelLoader {
public:
    static std::vector<Texture> textures_loaded;
    static void LoadModelToGameObject(Engine::GameObject* parent, const std::string& path);
private:
    static void processNode(Engine::GameObject* parent,const aiNode* node, const aiScene* scene, const std::string& directory);
    static void processMesh(Engine::GameObject* parent, aiMesh* mesh, const aiScene* scene, const std::string& directory);
    static std::vector<Texture> loadMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::string& directory);
};

} // Engine

#endif //MODELLOADER_H
