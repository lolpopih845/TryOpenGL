#pragma once
#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <assimp/scene.h>

#include "AssetObject.h"

namespace Asset {
    struct BoneInfo {
        int id = -1;
        glm::mat4 offsetMatrix{1.0f};
    };

    struct SkeletonNode {
        std::string name;
        glm::mat4 transform{1.0f};
        std::vector<std::unique_ptr<SkeletonNode>> children;
    };

    class Skeleton : public AssetObject{
    public:
        std::unordered_map<std::string, BoneInfo> bones;
        int boneCount = 0;
        std::vector<glm::mat4> inverseBindPoses;
        SkeletonNode rootNode;
        friend std::ostream& operator<<(std::ostream& os, const Skeleton& skeleton);

        inline Skeleton(const std::string &name, const std::string& path);

        BoneInfo* FindBone(const std::string& name) {
            try {
                auto it = bones.find(name);
                if (it != bones.end()) return &it->second;
            } catch (const std::exception& e) {
                std::cerr << "[Error] Exception in FindBone: " << e.what() << "\n";
            } catch (...) {
                std::cerr << "[Error] Unknown exception in FindBone\n";
            }
            std::cerr << "[Warning] Bone not found: " << name << "\n";
            return nullptr;
        }
    private:
        void LoadSkeleton(const std::string& filePath);

        static void ConvertAiNode(SkeletonNode& node,const aiNode* src );
    };

    inline std::ostream& operator<<(std::ostream& os, const Asset::BoneInfo& b) {
        os << "Bone ID: " << b.id << " | OffsetMatrix:\n";
        for (int i = 0; i < 4; ++i) {
            os << " [";
            for (int j = 0; j < 4; ++j) {
                os << b.offsetMatrix[i][j] ;
                if (j!=3) os << ", ";
            }
            os<<"]\n";
        }

        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const Asset::SkeletonNode& node) {
        os << "Node: " << node.name << "\n  Transform:\n";
        for (int i = 0; i < 4; ++i) {
            os << " [";
            for (int j = 0; j < 4; ++j) {
                os << node.transform[i][j] ;
                if (j!=3) os << ", ";
            }
            os<<"]\n";
        }
        for (const auto& child : node.children) {
            os << *child;
        }
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const Asset::Skeleton& skel) {
        os << "Skeleton Bone Count: " << skel.boneCount << "\n";
        os << "Bones:\n";
        for (const auto& [name, bone] : skel.bones) {
            os << " " << name << " -> " << bone;
        }
        os << "Root Node:\n" << skel.rootNode;
        return os;
    }
}



#endif //SKELETON_H
