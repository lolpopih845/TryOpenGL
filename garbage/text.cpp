//
// Created by User1 on 21/11/2025.
//

#include "text.h"
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Components {

    Text::Text(const unsigned int width, const unsigned int height)
    {
        // load and configure shader
        this->TextShader = ResourceManager::LoadShader("text_2d.vs", "text_2d.fs", nullptr, "text");
        this->TextShader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f), true);
        this->TextShader.SetInteger("text", 0);
        // configure VAO/VBO for texture quads
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
} // Components