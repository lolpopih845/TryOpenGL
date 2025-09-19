#include "texture.h"

#include <fstream>

#include <iostream>
#include "FileSystem.h"
#include <stb_image.h>

bool Texture::loadFromFile(const char* path,
                           const bool flipOnLoad,
                           const GLint wrapS, const GLint wrapT,
                           const GLint minFilter, const GLint magFilter)
{
    if (flipOnLoad) stbi_set_flip_vertically_on_load(1);
    else stbi_set_flip_vertically_on_load(0);

    unsigned char* data = stbi_load(FileSystem::getPath(path).c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << FileSystem::getPath(path).c_str() << "\n";
        return false;
    }

    // create GL texture if not already created
    if (ID == 0) glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // set parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    GLenum format = GL_RGB;
    if (channels == 1) format = GL_RED;
    else if (channels == 3) format = GL_RGB;
    else if (channels == 4) format = GL_RGBA;

    // upload
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ID);
}

Texture::Texture(const char *path, const std::string &type, const bool flip, const GLint wrapS, const GLint wrapT, const GLint minFilter, const GLint magFilter) {
    this->path = path;
    this->type = type;
    loadFromFile(path, flip, wrapS, wrapT, minFilter, magFilter);
}

Texture::~Texture()
{
    if (ID) glDeleteTextures(1, &ID);
}