//
// Created by User1 on 21/11/2025.
//

#include "StageDirector.h"
#include "Scene.h"
#include <glm/gtc/type_ptr.inl>

namespace Engine {
    void StageDirector::Init() {
        //Create UBO
        glGenBuffers(1, &stageUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, stageUBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(SceneData), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, stageUBO, 0, sizeof(SceneData));
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, stageUBO);

        glGenBuffers(1, &spotlightUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, spotlightUBO);
        size_t size = sizeof(Spotlight)*16 + sizeof(glm::vec4);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, spotlightUBO, 0, size);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, spotlightUBO);
    }

    void StageDirector::UpdateUBO() {
        if (stageUBO == 0) return;
        glBindBuffer(GL_UNIFORM_BUFFER, stageUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(data.ambientColor));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(data.dirLightDirection));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4)*2, sizeof(glm::vec4), glm::value_ptr(data.dirLightColor));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4)*3, sizeof(glm::vec4), glm::value_ptr(data.dirLightSpecular));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);


        if (spotlightUBO == 0) return;
        glBindBuffer(GL_UNIFORM_BUFFER, spotlightUBO);
        const size_t count = spotlights.size();
        glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(Spotlight) * count, spotlights.data());
        glm::vec4 meta(count,0,0,0);
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(Spotlight) * 32, sizeof(glm::vec4), glm::value_ptr(meta));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    int StageDirector::AddSpotlight(const Spotlight &spotlight) {
        spotlights.push_back(spotlight);
        return spotlights.size() - 1;
    }

} // Engine