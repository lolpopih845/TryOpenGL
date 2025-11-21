#ifndef MODELOBJECT_H
#define MODELOBJECT_H
#include "../Components/Model.h"
#include "../Engine/GameObject.h"

namespace Prefab {

    class ModelObject:public Engine::GameObject{
    public: 
        explicit ModelObject(const std::string &name, Engine::Transform transform = Engine::DEFAULT_TRANSFORM, Engine::GameObjectID parent = Engine::INVALID_ID): GameObject(name,transform, parent) {
            addComponent<Components::Model>();
        }
    };

} // Prefab


#endif //MODELOBJECT_H
