#pragma once 

#include <glm/glm.hpp>

namespace DWE {
    struct UniformData {
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        glm::mat4 ViewMatrix = glm::mat4(1.0f);
        glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
        glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
        glm::mat4 ModelViewProjectionMatrix = glm::mat4(1.0f);

        std::vector<char> getUniformData(uint8_t uniform_flags) const;
    };
}
