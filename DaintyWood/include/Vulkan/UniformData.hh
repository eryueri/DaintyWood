#pragma once 

#include <glm/glm.hpp>

namespace DWE {
    struct UniformData {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::mat4 ViewProjectionMatrix;
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
    };
}
