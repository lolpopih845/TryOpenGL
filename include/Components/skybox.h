#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>
#include "../Asset/shader.h"
#include "../Asset/texture.h"
#include <vector>
#include "component.h"
#include "transform.h"
#include "iostream"

namespace Components {

class Skybox : public Renderer{
public:
    Asset::Texture texture;
    unsigned int VAO;

    explicit Skybox(const Asset::Texture &texture);

    void setTexture(const Asset::Texture &texture);
    void updateMeshes() const;
    void render() override;
    const char *getComponentName() const override;
    friend std::ostream& operator<<(std::ostream& os, const Skybox& mesh);
private:
    std::vector<float> skyboxVertices;
    unsigned int VBO;

    void init() override;

};

} // Components

#endif //SKYBOX_H
