#include "Vulkan/ShaderSettings.hh"

#include <glm/glm.hpp>

namespace DWE {
    uint32_t vertexDataStride(VertexDataFlag flag)
    {
        switch (flag) {
        case VertexDataFlag::None: throw std::runtime_error("uninitialized vertex data type");
        case VertexDataFlag::Position: return sizeof(glm::vec3);
        case VertexDataFlag::Color: return sizeof(glm::vec4);
        case VertexDataFlag::Texcoord: return sizeof(glm::vec2);
        case VertexDataFlag::Normal: return sizeof(glm::vec3);
          break;
        }

        throw std::runtime_error("unknown vertex data type");
    }


    uint8_t operator| (const VertexDataFlag& a, const VertexDataFlag& b) { return static_cast<uint8_t>(a) | static_cast<uint8_t>(b); }
    uint8_t operator| (const uint8_t& a, const VertexDataFlag& b) { return a | static_cast<uint8_t>(b); }
    uint8_t operator| (const VertexDataFlag& a, const uint8_t& b) { return static_cast<uint8_t>(a) | b; }
    uint8_t operator& (const VertexDataFlag& a, const VertexDataFlag& b) { return static_cast<uint8_t>(a) & static_cast<uint8_t>(b); }
    uint8_t operator& (const uint8_t& a, const VertexDataFlag& b) { return a & static_cast<uint8_t>(b); }
    uint8_t operator& (const VertexDataFlag& a, const uint8_t& b) { return static_cast<uint8_t>(a) & b; }
    uint8_t operator|= (const VertexDataFlag& a, const VertexDataFlag& b) { return static_cast<uint8_t>(a) | static_cast<uint8_t>(b); }
    uint8_t operator|= (const uint8_t& a, const VertexDataFlag& b) { return a | static_cast<uint8_t>(b); }
    uint8_t operator|= (const VertexDataFlag& a, const uint8_t& b) { return static_cast<uint8_t>(a) | b; }
    uint8_t operator&= (const VertexDataFlag& a, const VertexDataFlag& b) { return static_cast<uint8_t>(a) & static_cast<uint8_t>(b); }
    uint8_t operator&= (const uint8_t& a, const VertexDataFlag& b) { return a & static_cast<uint8_t>(b); }
    uint8_t operator&= (const VertexDataFlag& a, const uint8_t& b) { return static_cast<uint8_t>(a) & b; }

    uint8_t operator| (const UniformFlag& a, const UniformFlag& b) { return static_cast<uint8_t>(a) | static_cast<uint8_t>(b); }
    uint8_t operator| (const uint8_t& a, const UniformFlag& b) { return a | static_cast<uint8_t>(b); }
    uint8_t operator| (const UniformFlag& a, const uint8_t& b) { return static_cast<uint8_t>(a) | b; }
    uint8_t operator& (const UniformFlag& a, const UniformFlag& b) { return static_cast<uint8_t>(a) & static_cast<uint8_t>(b); }
    uint8_t operator& (const uint8_t& a, const UniformFlag& b) { return a & static_cast<uint8_t>(b); }
    uint8_t operator& (const UniformFlag& a, const uint8_t& b) { return static_cast<uint8_t>(a) & b; }
    uint8_t operator|= (const UniformFlag& a, const UniformFlag& b) { return static_cast<uint8_t>(a) | static_cast<uint8_t>(b); }
    uint8_t operator|= (const uint8_t& a, const UniformFlag& b) { return a | static_cast<uint8_t>(b); }
    uint8_t operator|= (const UniformFlag& a, const uint8_t& b) { return static_cast<uint8_t>(a) | b; }
    uint8_t operator&= (const UniformFlag& a, const UniformFlag& b) { return static_cast<uint8_t>(a) & static_cast<uint8_t>(b); }
    uint8_t operator&= (const uint8_t& a, const UniformFlag& b) { return a & static_cast<uint8_t>(b); }
    uint8_t operator&= (const UniformFlag& a, const uint8_t& b) { return static_cast<uint8_t>(a) & b; }
}
