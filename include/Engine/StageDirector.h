#ifndef SCENEDIRECTOR_H
#define SCENEDIRECTOR_H

#include <vector>
#include <glm/detail/func_trigonometric.inl>

#include "glm/vec4.hpp"
#include <irrKlang/irrKlang.h>


namespace Engine {
    class Scene;

    struct SceneData {
        glm::vec4 ambientColor;
        glm::vec4 dirLightDirection;
        glm::vec4 dirLightColor;
        glm::vec4 dirLightSpecular;
    };
    struct Spotlight {
        glm::vec4 position;
        glm::vec4 direction;
        glm::vec4 ambientColor = glm::vec4(0.0f);
        glm::vec4 diffuseColor = glm::vec4(1.0f);
        glm::vec4 specularColor = glm::vec4(1.0f);
        glm::vec4 var = glm::vec4(1.0f,0.1f,0.032f,0); //constant, linear, quardratic
        glm::vec4 cutoff = glm::vec4(glm::cos(glm::radians(12.5f)),glm::cos(glm::radians(15.0f)),0,0);
    };
class StageDirector {
public:
    Scene* scene;
    void Init();
    void UpdateUBO();
    explicit StageDirector(const SceneData &data);
    SceneData& GetSceneData() { return data; }
    int AddSpotlight(const Spotlight &spotlight);
    Spotlight* GetSpotlight(const int idx) { return &spotlights[idx]; }
    void RemoveSpotlight(const int idx) { spotlights.erase(spotlights.begin() + idx); }
    irrklang::ISoundEngine* GetSoundEngine() const;

private:
    unsigned int stageUBO = 0;
    unsigned int spotlightUBO = 0;
    std::vector<Spotlight> spotlights;
    SceneData data;
    irrklang::ISoundEngine* engine;
};

} // Engine

#endif //SCENEDIRECTOR_H
