
#ifndef SOUND_H
#define SOUND_H
#include "AssetObject.h"
#include <irrKlang/irrKlang.h>
namespace Asset {
    class Sound : public AssetObject {
    public:
        Sound(const std::string& name, const std::string& path);
        void Play();
        void Stop();
    private:
        irrklang::ISoundEngine *soundEngine;
    };
}

#endif
