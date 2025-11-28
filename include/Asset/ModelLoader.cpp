//
// Created by User1 on 20/10/2025.
//

#include "ModelLoader.h"
#include <assimp/postprocess.h>
#include <iostream>

#include "../Components/animator.h"
#include "../BaseObject/Mesh.h"
#include "../garbage/AnimLoader.h"
#include "AssetStorage.h"
#include "../BaseObject/ModelObject.h"
#include "../Engine/EngineUtils.h"

namespace Asset {
    std::vector<Texture> ModelLoader::textures_loaded;

    void SetVertexBoneDataToDefault(Engine::Vertex& vertex)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            vertex.m_BoneIDs[i] = -1;
            vertex.m_Weights[i] = 0.0f;
        }
    }

    Engine::GameObject* ModelLoader::LoadModelGameObject(const std::string& name, const std::string& path,Shader* shader,bool hasBone) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices |
                aiProcess_GenSmoothNormals |
                aiProcess_LimitBoneWeights |
                aiProcess_OptimizeMeshes |
                aiProcess_OptimizeGraph);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return nullptr;
        }
        std::shared_ptr<Skeleton> skeleton = nullptr;
        if (hasBone) {
            AssetStorage<Skeleton>::Load(name,path);
        }

        const std::string directory = path.substr(0, path.find_last_of('/'));
        Engine::GameObject* rootGO = Game::Get(Game::CreateObject<Prefab::ModelObject>(name));
        processNode(name,rootGO,scene->mRootNode, scene, directory,shader, skeleton.get());
        rootGO->getComponent<Components::Model>()->UpdateMeshChildren();
        if (shader)
            rootGO->getComponent<Components::Model>()->SetShader(shader);
        if (skeleton)
            rootGO->addComponent<Components::Animator>();
        return rootGO;
    }

    void ModelLoader::processNode(const std::string& name, Engine::GameObject* parent,const aiNode* node, const aiScene* scene, const std::string& directory,Shader* shader, Skeleton* skeleton) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(name, parent, mesh, scene, directory,shader,skeleton);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(name, parent, node->mChildren[i], scene, directory,shader, skeleton);
        }
    }

    void ModelLoader::processMesh(const std::string& name, Engine::GameObject* parent, aiMesh *mesh, const aiScene *scene, const std::string &directory,Shader* shader,Skeleton* skeleton) {
        std::vector<Engine::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Engine::Vertex vertex;
            if (skeleton != nullptr) SetVertexBoneDataToDefault(vertex);
            vertex.Position = glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);
            if (mesh->HasNormals())
                vertex.Normal = glm::vec3(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
            if(mesh->mTextureCoords[0]) // texture coordinates
            {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);
                if (mesh->mTangents) vertex.Tangent = glm::vec3(mesh->mTangents[i].x,mesh->mTangents[i].y,mesh->mTangents[i].z);
                if (mesh->mBitangents) vertex.Bitangent = glm::vec3(mesh->mBitangents[i].x,mesh->mBitangents[i].y,mesh->mBitangents[i].z);
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        // faces
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // materials

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures(name, material, aiTextureType_DIFFUSE, "texture_diffuse",directory);
        std::vector<Texture> specularMaps = loadMaterialTextures(name, material, aiTextureType_SPECULAR, "texture_specular",directory);
        std::vector<Texture> normalMaps = loadMaterialTextures(name, material, aiTextureType_HEIGHT, "texture_normal",directory);
        std::vector<Texture> heightMaps = loadMaterialTextures(name, material, aiTextureType_AMBIENT, "texture_height",directory);

        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        if (skeleton != nullptr) ExtractBoneWeightForVertices(vertices,mesh,skeleton);

        if (skeleton)
            Engine::ObjectManager::CreateObject<Prefab::MeshObject>("mesh",parent, std::move(vertices), std::move(indices), std::move(textures),false,true);
        else
            Engine::ObjectManager::CreateObject<Prefab::MeshObject>("mesh",parent, std::move(vertices), std::move(indices), std::move(textures));
    }

    void SetVertexBoneData(Engine::Vertex& vertex, int boneID, float weight)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        {
            if (vertex.m_BoneIDs[i] < 0)
            {
                vertex.m_Weights[i] = weight;
                vertex.m_BoneIDs[i] = boneID;
                break;
            }
        }
    }

    void ModelLoader::ExtractBoneWeightForVertices(std::vector<Engine::Vertex>& vertices, const aiMesh* mesh, Skeleton* skeleton)
    {
        for (unsigned int b = 0; b < mesh->mNumBones; ++b)
        {
            aiBone* aiBonePtr = mesh->mBones[b];
            auto it = skeleton->bones.find(aiBonePtr->mName.C_Str());
            if (it == skeleton->bones.end()) continue;
            int boneID = it->second.id;
            assert(boneID != -1);
            for (unsigned int w = 0; w < aiBonePtr->mNumWeights; ++w)
            {
                unsigned int vertexId = aiBonePtr->mWeights[w].mVertexId;
                float weight = aiBonePtr->mWeights[w].mWeight;
                assert(vertexId <= vertices.size());
                SetVertexBoneData(vertices[vertexId], boneID, weight);
            }
        }
    }


    std::vector<Texture> ModelLoader::loadMaterialTextures(const std::string& name, const aiMaterial* mat, const aiTextureType type, const std::string& typeName, const std::string& directory)
    {
        std::vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string path = directory + "/" + str.C_Str();
            bool skip = false;
            for (auto& loaded : textures_loaded) {
                if (loaded.path == path) {
                    textures.push_back(loaded);
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                Texture texture(name+"_"+typeName,(path.c_str()), typeName);
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    };
} // Asset