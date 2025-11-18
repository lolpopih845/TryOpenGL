#ifndef MODELOBJECT_H
#define MODELOBJECT_H
#include "../Components/Model.h"
#include "../Engine/GameObject.h"

namespace Prefab {

    class ModelObject:public Engine::GameObject{
    public: // <- make constructor public
        explicit ModelObject(const std::string &name, GameObject* parent = nullptr): GameObject(name, parent) {
            addComponent<Components::Model>();
        }
    };

} // Prefab


#endif //MODELOBJECT_H
