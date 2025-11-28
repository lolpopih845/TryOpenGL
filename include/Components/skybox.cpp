//
// Created by User1 on 13/11/2025.
//

#include "skybox.h"

#include "../Engine/EngineUtils.h"

namespace Components {
    Skybox::Skybox(const Asset::Texture &texture): texture(texture), VAO(0), VBO(0) {
        skyboxVertices = {
            // positions
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
        };
        setTexture(texture);
    }

    void Skybox::setTexture(const Asset::Texture &texture) {
        this->texture = texture;
    }

    void Skybox::updateMeshes() const {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    }

    void Skybox::render() {
        if (!shader || VAO == 0) return;

        shader->use();
        shader->setInt("skybox", 0);
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        shader->setMat4("view", glm::mat4(glm::mat3(Game::GetCurrentCamera()->getComponent<Camera>()->GetViewMatrix())));
        // skybox cube
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture.ID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

    const char * Skybox::getComponentName() const {
        return "Skybox";
    }

    void Skybox::init() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        updateMeshes();
    }

    std::ostream & operator<<(std::ostream &os, const Skybox &mesh) {
        os << "Skybox (" << mesh.gameObject->name << "): "
        << "VAO: " << mesh.VAO << "VBO: " << mesh.VBO;
        os << " Texture: " << mesh.texture << std::endl;
        return os;
    }
} // Components