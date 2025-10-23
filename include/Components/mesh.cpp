#include <iostream>
#include <glad/glad.h> // holds all OpenGL type declarations

#include "mesh.h"

namespace Components {
    Mesh::Mesh(const std::vector<Engine::Vertex> &vertices, const std::vector<unsigned int> &indices,
    const std::vector<Asset::Texture> &textures, const bool use_tangent, const bool use_skinning) : vertices(std::move(vertices)),
      indices(std::move(indices)),textures(std::move(textures)),use_tangent(use_tangent),use_skinning(use_skinning),VAO(0), VBO(0), EBO(0) {}

    void Mesh::setupMesh(const std::vector<Engine::Vertex> &vertices_, const std::vector<unsigned int> &indices_, const std::vector<Asset::Texture> &textures_, bool use_tangent_, bool use_skinning_) {
        vertices = std::move(vertices_);
        indices = std::move(indices_);
        textures = std::move(textures_);
        use_tangent = use_tangent_;
        use_skinning = use_skinning_;
    }

    void Mesh::init()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        updateMeshes(this->use_tangent, this->use_skinning);
    }

    void Mesh::render() {
        if (!shader || VAO == 0 || indices.empty()) return;

        shader->use();
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);
            else if(name == "texture_normal")
                number = std::to_string(normalNr++);
            else if(name == "texture_height")
                number = std::to_string(heightNr++);
            glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
            textures[i].bind();
        }
        glBindVertexArray(VAO);
        if (gameObject) {
            if (const auto t = gameObject->getComponent<Transform>())
                shader->setMat4("model", t->getGlobalTransform().getMatrix());
        }
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }



void Mesh::updateMeshes(bool use_tangent_, bool use_skinning_) {
    use_tangent = use_tangent_;
    use_skinning = use_skinning_;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Engine::Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*) nullptr);

    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, Normal));

    // texcoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, TexCoords));

    // tangents/bitangents
    if (use_tangent) {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, Tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, Bitangent));
    } else {
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);
    }

    // bone IDs + weights
    if (use_skinning) {
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, m_BoneIDs));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, m_Weights));
    } else {
        glDisableVertexAttribArray(5);
        glDisableVertexAttribArray(6);
    }
    glBindVertexArray(0);
}
}
