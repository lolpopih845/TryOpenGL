#pragma once
#include <memory>
#include "glLib.h"
#include "FileSystem.h"

namespace Engine {
    class GameObject;
    class Scene;

    struct Transform {
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 getMatrix() const {
            auto m = glm::mat4(1.0f);
            m = glm::translate(m, translation);
            m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            m = glm::scale(m, scale);
            return m;
        }
    };

    const Transform DEFAULT_TRANSFORM({glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(.0f, 0.0f, 0.0f),glm::vec3(1.0f,1.0f,1.0f)});
    inline std::ostream& operator<<(std::ostream& os, const Transform& t) {
        os << "Translation: [" << t.translation[0] << ", " << t.translation[1] << ", " << t.translation[2] << "] "
        << "Rotation: [" << t.rotation[0] << ", " << t.rotation[1] << ", " << t.rotation[2] << "] "
        << "Scale: [" << t.scale[0] << ", " << t.scale[1] << ", " << t.scale[2] << "] ";
        return os;
    }

    enum class RenderPriority : int {
        BACKGROUND = 0,
        WORLD      ,
        SKYBOX     ,
        OBJECT     ,
        UI         ,
        COUNT
    };

    enum class CollisionLayer : int {
        STATIC_OBJECT = 0,
        PLAYER      ,
        ENEMY       ,
        PROJECTILE  ,
        PARTICLE    ,
        COLLECTABLE ,
        INTERACTABLE,
        COUNT
    };

    enum Tag {
        Default,
        Player,
        Enemy,
        Floor,
        Projectile,
        PlayerHitbox,
        Background,
        Ray
    };

    struct GameObjectID {
        uint32_t idx;
        uint32_t gen;
    };

    inline bool operator==(const GameObjectID& lhs, const GameObjectID& rhs) {
        return lhs.idx == rhs.idx && lhs.gen == rhs.gen;
    }

    struct GameObjectSlot {
        std::unique_ptr<GameObject> obj;
        uint32_t gen = 0;
    };

    static constexpr GameObjectID INVALID_ID{ UINT32_MAX, UINT32_MAX };

    inline bool IsIDValid(const GameObjectID id) {
        return id.idx != UINT32_MAX;
    }



}

namespace std {
    template<>
    struct hash<Engine::GameObjectID> {
        size_t operator()(const Engine::GameObjectID& id) const noexcept {
            return (static_cast<size_t>(id.idx) << 32) | id.gen;
        }
    };
}
