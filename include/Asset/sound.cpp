#include "sound.h"

#include "../Engine/EngineUtils.h"

namespace Asset {
    Sound::Sound(const std::string &name, const std::string &path,const float volume) : AssetObject(name, path, SOUND) {
        sound = Game::GetSoundEngine()->addSoundSourceFromFile(Engine::FileSystem::getPath(path).c_str());
        sound->setDefaultVolume(volume);
    }

    Sound::~Sound() {
        if (sound)
            sound->drop();
    }

    void Sound::setVolume(const float volume) const {
        sound->setDefaultVolume(volume);
    }
}
