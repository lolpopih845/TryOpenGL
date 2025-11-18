#ifndef ANIMATOR_H
#define ANIMATOR_H
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <iostream>

#include <memory>
#include "component.h"
#include "../Asset/animation.h"
#include "../Asset/skeleton.h"

namespace Components {
    class Animator : public Component{
    public:
        Animator(const std::shared_ptr<Asset::Skeleton> &skeleton = nullptr, Asset::Animation* animation = nullptr);
        void update(float dTime) override;
        void PlayAnimation(Asset::Animation* anim1, Asset::Animation* anim2 = nullptr, float time1 = 0, float time2 =0, float blend=0);
        const std::vector<glm::mat4>& GetFinalBoneMatrices() const
        {
            return m_FinalBoneMatrices;
        }

        void SetSkeleton(std::shared_ptr<Asset::Skeleton> skeleton);
        const char *getComponentName() const override;
        friend std::ostream& operator<<(std::ostream& os, const Animator& animator);
    private:

        glm::mat4 UpdateBlend(const Asset::Channel* c1, const Asset::Channel* c2);
        void CalculateBoneTransform(const Asset::SkeletonNode *node, const glm::mat4 &parentTransform);
        template<typename T>
        static int FindKeyIndex(const std::vector<T>& keys, float time);
        glm::vec3 InterpPosition(const Asset::Channel* c, float time);
        glm::quat InterpRotation(const Asset::Channel* c, float time);
        glm::vec3 InterpScale(const Asset::Channel* c, float time);

        std::vector<glm::mat4> m_FinalBoneMatrices;
        Asset::Animation* m_CurrentAnimation;
        Asset::Animation* m_CurrentAnimation2;
        std::shared_ptr<Asset::Skeleton> m_Skeleton = nullptr;
        float m_CurrentTime;
        float m_CurrentTime2;
        float m_DeltaTime;
        float m_blendAmount;
    };
}





#endif //ANIMATOR_H
