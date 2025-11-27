#ifndef TEXT_H
#define TEXT_H
#include <map>

#include "component.h"

namespace Components {
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // size of glyph
        glm::ivec2   Bearing;   // offset from baseline to left/top of glyph
        unsigned int Advance;   // horizontal offset to advance to next glyph
    };


class Text : public Renderer{
public:
    unsigned int VAO;

    std::map<char, Character> Characters;

    Asset::Shader* TextShader;

    Text(unsigned int width, unsigned int height);

    void Load(std::string font, unsigned int fontSize);

    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
private:
    unsigned int VBO;
};

} // Components

#endif //TEXT_H
