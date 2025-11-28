
#ifndef SOUND_H
#define SOUND_H
#include "AssetObject.h"
#include <irrKlang/irrKlang.h>
namespace Asset {
    class Sound : public AssetObject {
    public:
        irrklang::ISoundSource* sound;
        Sound(const std::string& name, const std::string& path,float volume  = 1);
        ~Sound() override;
        void setVolume(float volume) const;
    };
}

#endif
