#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <glad/glad.h>

#include "AssetObject.h"

namespace Asset {
    class Texture:public AssetObject{
    private:
        int width = 0, height = 0, channels = 0;

    public:
        unsigned int ID;
        const char *path;
        std::string type;
        Texture(const std::string &name,const std::vector<std::string>& faces,bool flipOnLoad = true,
                          GLint wrapS = GL_CLAMP_TO_EDGE, GLint wrapT = GL_CLAMP_TO_EDGE, GLint wrapR = GL_CLAMP_TO_EDGE,
                          GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR);
        Texture(const std::string &name,const char* path, const std::string &type = "texture_diffuse", bool flip = true, GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT,GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_LINEAR);
        ~Texture() override;
        bool loadFromFile(const char* path,
                          bool flipOnLoad = true,
                          GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT,
                          GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_LINEAR);
        bool loadCubemap(const std::vector<std::string>& faces,bool flipOnLoad = false,
                          GLint wrapS = GL_REPEAT, GLint wrapT =GL_REPEAT, GLint wrapR = GL_REPEAT,
                          GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);
        void bind() const;
        friend std::ostream& operator<<(std::ostream& os, const Texture& texture);
    };
    Texture& DefaultSkyBoxTexture();
}



#endif //TEXTURE_H
