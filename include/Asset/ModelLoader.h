#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "../Components/mesh.h"
#include "shader.h"
#include "texture.h"

#include <string>
#include <fstream>
#include <vector>
#include "../Asset/skeleton.h"

namespace Asset {
class ModelLoader {
public:
    static std::vector<Texture> textures_loaded;
    static Engine::GameObject* LoadModelGameObject(const std::string& name, const std::string& path,Shader* shader = nullptr,bool hasBone = false);
private:
    static void processNode(const std::string& name,Engine::GameObject* parent,const aiNode* node, const aiScene* scene, const std::string& directory,Shader* shader, Skeleton* skeleton);
    static void processMesh(const std::string& name,Engine::GameObject* parent, aiMesh* mesh, const aiScene* scene, const std::string& directory,Shader* shader, Skeleton* skeleton);

    static void ExtractBoneWeightForVertices(std::vector<Engine::Vertex>& vertices, aiMesh* mesh, Skeleton* skeleton);

    static std::vector<Texture> loadMaterialTextures(const std::string& name,const aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::string& directory);
};

} // Engine

#endif //MODELLOADER_H
