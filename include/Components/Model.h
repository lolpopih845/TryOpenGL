//
// Created by User1 on 4/11/2025.
//

#ifndef MODEL_H
#define MODEL_H
#include <vector>

#include "../BaseObject/Mesh.h"

namespace Components {

class Model : public Component{
public:
    void UpdateMeshChildren();
    Engine::GameObject* GetMesh(unsigned int index) const;
    std::vector<Engine::GameObject*> GetMeshes();
    void SetShader(Asset::Shader* shader);
    Asset::Shader* GetShader() const;
    void AddMesh(Engine::GameObject* mesh);
    const char *getComponentName() const override;
private:

    std::vector<Engine::GameObject*> meshes;
    Asset::Shader* shader = &Asset::DefaultShader();
};

} // Components

#endif //MODEL_H
