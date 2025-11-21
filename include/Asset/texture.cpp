#include "../Asset/texture.h"
#include <iostream>
#include <stb_image.h>

namespace Asset {
    bool Texture::loadFromFile(const char* path,
                           const bool flipOnLoad,
                           const GLint wrapS, const GLint wrapT,
                           const GLint minFilter, const GLint magFilter)
{
    if (flipOnLoad) stbi_set_flip_vertically_on_load(1);
    else stbi_set_flip_vertically_on_load(0);

    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << path << "\n";
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

bool Texture::loadCubemap(const std::vector<std::string>& faces,const bool flipOnLoad,
                           const GLint wrapS, const GLint wrapT, const GLint wrapR,
                           const GLint minFilter, const GLint magFilter)
{
    if (flipOnLoad) stbi_set_flip_vertically_on_load(1);
    else stbi_set_flip_vertically_on_load(0);

    if (ID == 0) glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        if (unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0))
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ID);
}

Texture::Texture(const std::string &name,const char *path, const std::string &type, const bool flip, const GLint wrapS, const GLint wrapT, const GLint minFilter, const GLint magFilter) : AssetObject(name,TEXTURE){
    this->path = path;
    this->type = type;
    loadFromFile(path, flip, wrapS, wrapT, minFilter, magFilter);
}

Texture::Texture(const std::string &name,const std::vector<std::string>& faces,bool flipOnLoad,
                      GLint wrapS, GLint wrapT, GLint wrapR,
                      GLint minFilter, GLint magFilter) : AssetObject(name,TEXTURE) {
    this->path = faces[0].c_str();
    this->type = type;
    loadCubemap(faces, flipOnLoad, wrapS, wrapT, wrapR, minFilter, magFilter);
}

Texture::~Texture()
{
    if (ID) glDeleteTextures(1, &ID);
}
std::ostream& operator<<(std::ostream& os, const Texture& texture) {
    os << "Texture : " << texture.name
        << " ID: " << texture.ID
       << " path: " << texture.path
       << " type: " << texture.type
        << " width: " << texture.width
       << " height: " << texture.height
       << " channels: " << texture.channels;
    return os;
}
    Texture & DefaultSkyBoxTexture() {
        std::vector<std::string> f = {
            "resources/sky3.jpg",
            "resources/sky3.jpg",
            "resources/sky1.jpg",
            "resources/sky1.jpg",
            "resources/sky2.jpg",
            "resources/sky2.jpg"};
        static Texture defaultSkyBoxTexture("DefaultSkyBox",f);
        return defaultSkyBoxTexture;
    }
}
