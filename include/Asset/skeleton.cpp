#include "skeleton.h"
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

namespace Asset {
    Skeleton::Skeleton(const std::string &name, const std::string& path) :AssetObject(name,SKELETON){
        LoadSkeleton(path);
    }

    void Skeleton::ConvertAiNode(SkeletonNode &node, const aiNode *src) {
        node.name = src->mName.C_Str();
        node.transform = glm::transpose(glm::make_mat4(&src->mTransformation.a1));

        node.children.reserve(src->mNumChildren);
        for (unsigned int i = 0; i < src->mNumChildren; ++i)
        {
            auto child = std::make_unique<SkeletonNode>();
            ConvertAiNode(*child, src->mChildren[i]);
            node.children.push_back(std::move(child));
        }
    }

    void Skeleton::LoadSkeleton(const std::string &filePath) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_GenSmoothNormals |
    aiProcess_LimitBoneWeights |
    aiProcess_GlobalScale |
    aiProcess_OptimizeMeshes |
    aiProcess_OptimizeGraph);

        if (!scene || !scene->mRootNode) {
            throw std::runtime_error("Failed to load model: " + filePath);
        }

        ConvertAiNode(rootNode,scene->mRootNode);

        for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
            aiMesh* mesh = scene->mMeshes[m];
            for (unsigned int b = 0; b < mesh->mNumBones; ++b) {
                aiBone* aiBonePtr = mesh->mBones[b];
                std::string boneName = aiBonePtr->mName.C_Str();
                if (bones.find(boneName) == bones.end()) {
                    BoneInfo info;
                    info.id = boneCount++;
                    info.offsetMatrix = glm::transpose(glm::make_mat4(&aiBonePtr->mOffsetMatrix.a1));
                    bones[boneName] = info;
                    inverseBindPoses.push_back(info.offsetMatrix);
                }
            }
        }
    }


}
