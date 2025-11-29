#ifndef MODELOBJECT_H
#define MODELOBJECT_H
#include "../Asset/ModelLoader.h"
#include "../Components/Model.h"
#include "../Engine/GameObject.h"

namespace Prefab {

    class ModelObject:public Engine::GameObject {
    public:
        explicit ModelObject(const Engine::GameObjectID id,const std::string &name,const std::string& path, const bool hasBone = false,
            const Engine::Transform &transform = Engine::DEFAULT_TRANSFORM, Engine::GameObjectID parent = Engine::INVALID_ID): GameObject(id, name,transform, parent){
            addComponent<Components::Model>();
            Asset::Skeleton* skeleton = nullptr;
            if (hasBone)
                skeleton = Asset::AssetStorage<Asset::Skeleton>::Get(name);
            Asset::ModelLoader::LoadModelGameObject(this,name,path,&Asset::DefaultShader(),skeleton);
        }
    };

} // Prefab


#endif //MODELOBJECT_H
