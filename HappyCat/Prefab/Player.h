#ifndef PLAYER_H
#define PLAYER_H

#include "../../include/Asset/AssetStorage.h"
#include "../../include/BaseObject/ModelObject.h"

using namespace Components;
using namespace Asset;

namespace Prefab {

    class Player: public ModelObject{
    public:
        explicit Player(const Engine::GameObjectID id,const std::string &name,const Engine::Transform &transform = Engine::DEFAULT_TRANSFORM, Engine::GameObjectID parent = Engine::INVALID_ID):
        ModelObject(id,name,"",true,transform, parent) {
            addComponent<Collider>(glm::vec3(0),glm::vec3(1));
            Mesh* mesh = getComponent<Mesh>();
            mesh->textures.push_back(AssetStorage<Texture>::Get("Floor"));
        }
    };
}

#endif //PLAYER_H
