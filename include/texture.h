#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>

class Texture {
private:
    int width = 0, height = 0, channels = 0;

public:
    unsigned int ID;
    const char *path;
    std::string type;
    Texture(const char* path, const std::string &type = "texture_diffuse", bool flip = true, GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT,GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_LINEAR);
    ~Texture();
    bool loadFromFile(const char* path,
                      bool flipOnLoad = true,
                      GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT,
                      GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_LINEAR);
    void bind() const;
};



#endif //TEXTURE_H
