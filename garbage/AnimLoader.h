    #ifndef ANIMLOADER_H
#define ANIMLOADER_H
#include <memory>
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Asset/skeleton.h"
#include "../Asset/animation.h"

namespace Asset {
    inline void ConvertAiNode(SkeletonNode& node,const aiNode* src ) {
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

    inline std::shared_ptr<Skeleton> LoadSkeleton(const std::string& name, const std::string& filePath)
    {
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

        auto skeleton = std::make_shared<Skeleton>(name);

        ConvertAiNode(skeleton->rootNode,scene->mRootNode);

        for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
            aiMesh* mesh = scene->mMeshes[m];
            for (unsigned int b = 0; b < mesh->mNumBones; ++b) {
                aiBone* aiBonePtr = mesh->mBones[b];
                std::string boneName = aiBonePtr->mName.C_Str();
                if (skeleton->bones.find(boneName) == skeleton->bones.end()) {
                    BoneInfo info;
                    info.id = skeleton->boneCount++;
                    info.offsetMatrix = glm::transpose(glm::make_mat4(&aiBonePtr->mOffsetMatrix.a1));
                    skeleton->bones[boneName] = info;
                    skeleton->inverseBindPoses.push_back(info.offsetMatrix);
                }
            }
        }

        return skeleton;
    }

    inline std::shared_ptr<Animation> LoadAnimation(const std::string& name, const std::string& path){
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_GenSmoothNormals |
    aiProcess_LimitBoneWeights |
    aiProcess_GlobalScale |
    aiProcess_OptimizeMeshes |
    aiProcess_OptimizeGraph);
        if (!scene || !scene->mRootNode || scene->mNumAnimations == 0)
            throw std::runtime_error("Failed to load animation: " + path);

        auto clip = std::make_shared<Animation>(name);
        const aiAnimation* animation = scene->mAnimations[0];

        clip->duration = static_cast<float>(animation->mDuration);
        clip->ticksPerSecond = (animation->mTicksPerSecond != 0.0) ? static_cast<float>(animation->mTicksPerSecond) : 25.0f;

        for (unsigned int i = 0; i < animation->mNumChannels; ++i) {
            const aiNodeAnim* channel = animation->mChannels[i];
            Channel ch;
            ch.boneName = channel->mNodeName.C_Str();

            // positions
            for (unsigned int k = 0; k < channel->mNumPositionKeys; ++k) {
                KeyPosition kp;
                kp.position = glm::vec3(channel->mPositionKeys[k].mValue.x,
                                        channel->mPositionKeys[k].mValue.y,
                                        channel->mPositionKeys[k].mValue.z);
                kp.timeStamp = static_cast<float>(channel->mPositionKeys[k].mTime);
                ch.positions.push_back(kp);
            }

            // rotations
            for (unsigned int k = 0; k < channel->mNumRotationKeys; ++k) {
                KeyRotation kr;
                aiQuaternion q = channel->mRotationKeys[k].mValue;
                kr.orientation = glm::quat(q.w, q.x, q.y, q.z);
                kr.timeStamp = static_cast<float>(channel->mRotationKeys[k].mTime);
                ch.rotations.push_back(kr);
            }

            // scales
            for (unsigned int k = 0; k < channel->mNumScalingKeys; ++k) {
                KeyScale ks;
                ks.scale = glm::vec3(channel->mScalingKeys[k].mValue.x,
                                     channel->mScalingKeys[k].mValue.y,
                                     channel->mScalingKeys[k].mValue.z);
                ks.timeStamp = static_cast<float>(channel->mScalingKeys[k].mTime);
                ch.scales.push_back(ks);
            }

            clip->channels.push_back(ch);
        }

        return clip;
    }
} // Asset

#endif //ANIMLOADER_H
