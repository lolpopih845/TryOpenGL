#ifndef FLOOR_H
#define FLOOR_H
#include "Box.h"
#include "../../include/Asset/AssetStorage.h"

using namespace Components;

namespace Prefab {

    class Floor:public Box {
        public:
        explicit Floor(
            const Engine::GameObjectID id, const std::string &name, const Engine::Transform &transform = Engine::DEFAULT_TRANSFORM, const Engine::GameObjectID parent = Engine::INVALID_ID
            ):Box(id,name,transform,parent) {
            Mesh* mesh = getComponent<Mesh>();
            mesh->textures.push_back(Asset::AssetStorage<Asset::Texture>::Get("Floor"));
        }
    };
}

#endif //FLOOR_H
