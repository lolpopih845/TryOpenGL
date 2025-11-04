#pragma once
#ifndef SKELETON_H
#define SKELETON_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>

namespace Asset {
    struct BoneInfo {
        int id;
        glm::mat4 offsetMatrix;
    };

    struct SkeletonNode {
        std::string name;
        glm::mat4 transform;
        std::vector<std::unique_ptr<SkeletonNode>> children;
    };

    class Skeleton {
    public:
        std::unordered_map<std::string, BoneInfo> bones;
        int boneCount = 0;
        std::vector<glm::mat4> inverseBindPoses;
        SkeletonNode rootNode;


        BoneInfo* FindBone(const std::string& name) {
            auto it = bones.find(name);
            if (it != bones.end()) return &it->second;
            return nullptr;
        }
    };
}


#endif //SKELETON_H
