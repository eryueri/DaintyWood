#include "Vulkan/ShaderSettings.hh"

namespace DWE {
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
