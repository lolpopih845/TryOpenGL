#include "SoundSource.h"

#include "transform.h"
#include "../Engine/EngineUtils.h"
#include "../Engine/GameObject.h"

namespace Components {
    SoundSource::SoundSource(Asset::Sound *sound, bool isLooping, float minDistance) : sound(sound),
        isLooping(isLooping), minDistance(minDistance), curr_sounds(nullptr) {
    }

    void SoundSource::Play() {
        const glm::vec3 pos = gameObject->getComponent<Transform>()->getGlobalTransform().translation;
        const irrklang::vec3df position = {pos.x,pos.y,pos.z};
        sound->sound->setDefaultMinDistance(minDistance);
        curr_sounds = Game::GetSoundEngine()->play3D(sound->sound, position,isLooping,false);
        if (curr_sounds)
            curr_sounds->setPosition(position);
    }

    void SoundSource::Stop() const {
        Game::GetSoundEngine()->stopAllSoundsOfSoundSource(sound->sound);
    }

    void SoundSource::update(const float dTime) {
        const glm::vec3 pos = gameObject->getComponent<Transform>()->getGlobalTransform().translation;
        const irrklang::vec3df position = {pos.x,pos.y,pos.z};
        if (curr_sounds)
            curr_sounds->setPosition(position);
    }
} // Components