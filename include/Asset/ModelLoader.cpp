//
// Created by User1 on 20/10/2025.
//

#include "ModelLoader.h"
#include "../Engine/ObjectManager.h"
#include <assimp/postprocess.h>
#include <iostream>

#include "../BaseObject/Mesh.h"

namespace Asset {
    std::vector<Texture> ModelLoader::textures_loaded;

    void ModelLoader::LoadModelToGameObject(Engine::GameObject* parent, const std::string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        const std::string directory = path.substr(0, path.find_last_of('/'));
        processNode(parent, scene->mRootNode, scene, directory);
    }

    void ModelLoader::processNode(Engine::GameObject* parent,const aiNode* node, const aiScene* scene, const std::string& directory) {
        Engine::GameObject* rootGO = Engine::ObjectManager::CreateObject<Engine::GameObject>(parent);
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(rootGO, mesh, scene, directory);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(rootGO, node->mChildren[i], scene, directory);
        }
    }

    void ModelLoader::processMesh(Engine::GameObject* parent, aiMesh *mesh, const aiScene *scene, const std::string &directory) {
        std::vector<Engine::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Engine::Vertex vertex;
            glm::vec3 vector; // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            if (mesh->HasNormals()) // normals
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            if(mesh->mTextureCoords[0]) // texture coordinates
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
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

        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse",directory);
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular",directory);
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal",directory);
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height",directory);

        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        Engine::ObjectManager::CreateObject<Prefab::MeshObject>(parent, std::move(vertices), std::move(indices), std::move(textures));
    }
    std::vector<Texture> ModelLoader::loadMaterialTextures(const aiMaterial* mat, const aiTextureType type, const std::string& typeName, const std::string& directory)
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
                Texture texture((path.c_str()), typeName);
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    };
} // Engine