#pragma once
#include <memory>

#include "glLib.h"

namespace Engine {
    class GameObject;
    class Scene;

    enum class RenderPriority : int {
        BACKGROUND = 0,
        WORLD      ,
        SKYBOX     ,
        OBJECT     ,
        UI         ,
        COUNT
    };

    enum Tag {
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

    struct GameObjectSlot {
        std::unique_ptr<GameObject> obj;
        uint32_t gen = 0;
    };

    static constexpr GameObjectID INVALID_ID{ UINT32_MAX, UINT32_MAX };

    inline bool IsIDValid(const GameObjectID id) {
        return id.idx != UINT32_MAX;
    }



}
