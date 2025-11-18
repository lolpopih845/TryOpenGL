#ifndef MODELOBJECT_H
#define MODELOBJECT_H
#include "../Components/Model.h"
#include "../Engine/GameObject.h"

namespace Prefab {

    class ModelObject:public Engine::GameObject{
    public: // <- make constructor public
        explicit ModelObject(const GameObject* parent = nullptr): GameObject(*parent) {
            addComponent<Components::Model>();
        }
    };

} // Prefab


#endif //MODELOBJECT_H
