#include "sound.h"

namespace Asset {
    Sound::Sound(const std::string &name, const std::string &path) : AssetObject(name, path, SOUND) {
        //soundEngine = irrklang::createIrrKlangDevice(); -----> One people centralize
    }
}
