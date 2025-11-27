#include "animation.h"
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace Asset {

    Animation::Animation(const std::string &name, const std::string &path) : AssetObject(name,path,ANIMATION){
        LoadAnimation(path);
    }

    void Animation::LoadAnimation(const std::string& path){
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_GenSmoothNormals |
    aiProcess_LimitBoneWeights |
    aiProcess_OptimizeMeshes |
    aiProcess_OptimizeGraph);
        if (!scene || !scene->mRootNode || scene->mNumAnimations == 0)
            throw std::runtime_error("Failed to load animation: " + path);

        const aiAnimation* animation = scene->mAnimations[0];

        duration = static_cast<float>(animation->mDuration);
        ticksPerSecond = (animation->mTicksPerSecond != 0.0) ? static_cast<float>(animation->mTicksPerSecond) : 25.0f;

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

            channels.push_back(ch);
        }
    }



    Channel* Animation::FindChannel(const std::string& boneName) {
        try {
            for (auto& c : channels) {
                if (c.boneName == boneName)
                    return &c;
            }
        } catch (const std::exception& e) {
            std::cerr << "[Error] Exception in FindChannel: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "[Error] Unknown exception in FindChannel" << std::endl;
        }

        std::cerr << "[Warning] Channel not found for bone: " << boneName << std::endl;
        return nullptr;
    }

    std::ostream& operator<<(std::ostream& os, const KeyPosition& kp) {
        os << "Position(" << kp.position.x << ", " << kp.position.y << ", " << kp.position.z
           << ") Time: " << kp.timeStamp;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const KeyRotation& kr) {
        os << "Rotation(" << kr.orientation.w << ", " << kr.orientation.x << ", "
           << kr.orientation.y << ", " << kr.orientation.z << ") Time: " << kr.timeStamp;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const KeyScale& ks) {
        os << "Scale(" << ks.scale.x << ", " << ks.scale.y << ", " << ks.scale.z
           << ") Time: " << ks.timeStamp;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Channel& ch) {
        os << "Bone: " << ch.boneName << "\n";
        os << " Positions:\n";
        for (const auto& p : ch.positions) os << "  " << p << "\n";
        os << " Rotations:\n";
        for (const auto& r : ch.rotations) os << "  " << r << "\n";
        os << " Scales:\n";
        for (const auto& s : ch.scales) os << "  " << s << "\n";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Animation& anim) {
        os << "Animation : " << anim.name << "\n"
            << "Animation Duration: " << anim.duration << "\n"
           << " TicksPerSecond: " << anim.ticksPerSecond << "\n"
           << " Channels: " << anim.channels.size() << "\n";
        for (const auto& ch : anim.channels) os << ch << "\n";
        return os;
    }
}
