
#include "animator.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Model.h"
#include "../Engine/GameObject.h"

namespace Components {
    Animator::Animator(const std::shared_ptr<Asset::Skeleton> &skeleton, Asset::Animation* animation)
        : m_CurrentAnimation(animation), m_CurrentAnimation2(nullptr),
          m_Skeleton(skeleton.get()), m_CurrentTime(0.0f), m_CurrentTime2(0), m_DeltaTime(0),
          m_blendAmount(0.0f) {
        m_FinalBoneMatrices.resize(m_Skeleton ? m_Skeleton->bones.size() : 100, glm::mat4(1.0f));
    }
    void Animator::SetSkeleton(std::shared_ptr<Asset::Skeleton> skeleton)
    {
        assert(skeleton && "SetSkeleton received null skeleton!");
        m_Skeleton = skeleton;
        m_FinalBoneMatrices.resize(skeleton->bones.size(), glm::mat4(1.0f));
    }
    void Animator::PlayAnimation(Asset::Animation* anim1, Asset::Animation* anim2, const float time1, const float time2, const float blend)
    {
        m_CurrentAnimation = anim1;
        m_CurrentTime = time1;
        m_CurrentAnimation2 = anim2;
        m_CurrentTime2 = time2;
        m_blendAmount = blend;
    }

    void Animator::update(float deltaTime)
    {
        if (!m_CurrentAnimation || !m_Skeleton) {return;}

        m_CurrentTime += m_CurrentAnimation->ticksPerSecond * deltaTime;
        m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->duration);

        if (m_CurrentAnimation2)
        {
            m_CurrentTime2 += m_CurrentAnimation2->ticksPerSecond * deltaTime;
            m_CurrentTime2 = fmod(m_CurrentTime2, m_CurrentAnimation2->duration);
        }

        CalculateBoneTransform(&m_Skeleton->rootNode, glm::mat4(1.0f));
        auto transforms = GetFinalBoneMatrices();
        auto model = gameObject->getComponent<Model>();
        auto meshGOs = model->GetMeshes(); // returns vector<GameObject*>

        for (auto* meshGO : meshGOs)
        {
            if (!meshGO) continue;
            auto* meshComp = meshGO->getComponent<Mesh>();
            if (!meshComp) continue;
            Asset::Shader* sh = meshComp->shader;
            if (!sh) continue;

            sh->use();
            sh->setMat4Array("finalBonesMatrices", transforms.data(), (GLuint)transforms.size());
        }
    }

    glm::mat4 Animator::UpdateBlend(const Asset::Channel* c1, const Asset::Channel* c2)
    {
        glm::vec3 pos1 = c1 ? InterpPosition(c1, m_CurrentTime) : glm::vec3(0);
        glm::quat rot1 = c1 ? InterpRotation(c1, m_CurrentTime) : glm::quat();
        glm::vec3 scale1 = c1 ? InterpScale(c1, m_CurrentTime) : glm::vec3(1);

        glm::vec3 pos2 = c2 ? InterpPosition(c2, m_CurrentTime2) : pos1;
        glm::quat rot2 = c2 ? InterpRotation(c2, m_CurrentTime2) : rot1;
        glm::vec3 scale2 = c2 ? InterpScale(c2, m_CurrentTime2) : scale1;

        glm::vec3 posFinal   = glm::mix(pos1, pos2, m_blendAmount);
        glm::quat rotFinal   = glm::normalize(glm::slerp(rot1, rot2, m_blendAmount));
        glm::vec3 scaleFinal = glm::mix(scale1, scale2, m_blendAmount);

        return glm::translate(glm::mat4(1.0f), posFinal) *
               glm::toMat4(rotFinal) *
               glm::scale(glm::mat4(1.0f), scaleFinal);
    }

    void Animator::CalculateBoneTransform(const Asset::SkeletonNode *node, const glm::mat4 &parentTransform) {
        glm::mat4 nodeTransform = node->transform;

        // Find channel(s)
        const Asset::Channel* ch1 = nullptr;
        const Asset::Channel* ch2 = nullptr;

        if (m_CurrentAnimation) {
            for (auto& c : m_CurrentAnimation->channels)
                if (c.boneName == node->name) { ch1 = &c; break; }
        }
        if (m_CurrentAnimation2) {
            for (auto& c : m_CurrentAnimation2->channels)
                if (c.boneName == node->name) { ch2 = &c; break; }
        }

        if (ch1 && !ch2)
        {
            glm::vec3 pos  = InterpPosition(ch1, m_CurrentTime);
            glm::quat rot  = InterpRotation(ch1, m_CurrentTime);
            glm::vec3 scale= InterpScale(ch1, m_CurrentTime);

            nodeTransform =
                glm::translate(glm::mat4(1.0f), pos) *
                glm::toMat4(rot) *
                glm::scale(glm::mat4(1.0f), scale);
        }
        else if (ch1)
        {
            nodeTransform = UpdateBlend(ch1, ch2);
        }

        glm::mat4 globalTransform = parentTransform * nodeTransform;

        // Apply to bone if exists
        auto it = m_Skeleton->bones.find(node->name);
        if (it != m_Skeleton->bones.end()) {
            int index = it->second.id;
            glm::mat4 offset = it->second.offsetMatrix;
            if (index >= 0 && index < (int)m_FinalBoneMatrices.size())
                m_FinalBoneMatrices[index] = globalTransform * offset;
        }

        for (const auto& child : node->children)
            CalculateBoneTransform(child.get(), globalTransform);
    }


    template <typename T>
    int Animator::FindKeyIndex(const std::vector<T>& keys, float time) {
        for (int i = 0; i < (int)keys.size() - 1; i++) {
            if (time < keys[i + 1].timeStamp)
                return i;
        }
        return (int)keys.size() - 2;
    }
    glm::vec3 Animator::InterpPosition(const Asset::Channel* c, float time) {
        if (!c || c->positions.empty()) return glm::vec3(0);

        if (c->positions.size() == 1) return c->positions[0].position;

        int i = FindKeyIndex(c->positions, time);
        int j = i + 1;

        float dt = c->positions[j].timeStamp - c->positions[i].timeStamp;
        float t = (time - c->positions[i].timeStamp) / dt;

        return glm::mix(c->positions[i].position, c->positions[j].position, t);
    }
    glm::quat Animator::InterpRotation(const Asset::Channel* c, float time) {
        if (!c || c->rotations.empty()) return glm::quat(1,0,0,0);

        if (c->rotations.size() == 1) return glm::normalize(c->rotations[0].orientation);

        int i = FindKeyIndex(c->rotations, time);
        int j = i + 1;

        float dt = c->rotations[j].timeStamp - c->rotations[i].timeStamp;
        float t = (time - c->rotations[i].timeStamp) / dt;

        return glm::normalize(glm::slerp(
            c->rotations[i].orientation,
            c->rotations[j].orientation,
            t
        ));
    }
    glm::vec3 Animator::InterpScale(const Asset::Channel* c, float time) {
        if (!c || c->scales.empty()) return glm::vec3(1);

        if (c->scales.size() == 1) return c->scales[0].scale;

        int i = FindKeyIndex(c->scales, time);
        int j = i + 1;

        float dt = c->scales[j].timeStamp - c->scales[i].timeStamp;
        float t = (time - c->scales[i].timeStamp) / dt;

        return glm::mix(c->scales[i].scale, c->scales[j].scale, t);
    }
}

