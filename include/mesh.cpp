#include <iostream>
#include <glad/glad.h> // holds all OpenGL type declarations

#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures, const Transform &transform, Mesh *parent, bool use_tangent, bool use_skinning): parent() {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->parent = parent;
    this->transform = transform;
    this->use_tangent = use_tangent;
    this->use_skinning = use_skinning;
    setupMesh();
}

void Mesh::Draw(const Shader &shader) const {
    shader.use();
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
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        textures[i].bind();
    }
    glBindVertexArray(VAO);
    shader.setMat4("model", getGlobalTransform().getMatrix());
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    updateMeshes(this->use_tangent, this->use_skinning);
}

void Mesh::updateMeshes(bool use_tangent, bool use_skinning) {
    this->use_tangent = use_tangent;
    this->use_skinning = use_skinning;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) nullptr);

    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // texcoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    // tangents/bitangents
    if (use_tangent) {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    } else {
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);
    }

    // bone IDs + weights
    if (use_skinning) {
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    } else {
        glDisableVertexAttribArray(5);
        glDisableVertexAttribArray(6);
    }
    glBindVertexArray(0);
}

Transform Mesh::getGlobalTransform() const{
    if (!parent) return transform;
    auto p = parent->getGlobalTransform();
    return Transform({
         glm::quat(glm::radians(p.rotation)) * (p.scale * transform.translation) + p.translation,
        glm::degrees(glm::eulerAngles(glm::quat(glm::radians(p.rotation)) * glm::quat(glm::radians(transform.rotation)))),
        p.scale * transform.scale
    });
}