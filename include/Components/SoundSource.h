#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H
#include "component.h"
#include "../Asset/sound.h"

namespace Components {

class SoundSource : public Component{
public:
    Asset::Sound* sound;
    bool isLooping;
    float minDistance = 0;
    explicit SoundSource(Asset::Sound* sound = nullptr,bool isLooping = false,float minDistance = 0);
    void Play();
    void Stop() const;

    void update(float dTime) override;

private:
    irrklang::ISound* curr_sounds;
};

} // Components

#endif //SOUNDSOURCE_H
