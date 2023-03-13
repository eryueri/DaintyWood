#include "Vulkan/ShaderSettings.hh"

namespace DWE {
    uint8_t operator| (const VertexDataFlags& a, const VertexDataFlags& b) { return static_cast<uint8_t>(a) | static_cast<uint8_t>(b); }
    uint8_t operator& (const VertexDataFlags& a, const VertexDataFlags& b) { return static_cast<uint8_t>(a) & static_cast<uint8_t>(b); }
    uint8_t operator& (const uint8_t& a, const VertexDataFlags& b) { return a & static_cast<uint8_t>(b); }
    uint8_t operator& (const VertexDataFlags& a, const uint8_t& b) { return static_cast<uint8_t>(a) & b; }
    uint8_t operator| (const UniformFlags& a, const UniformFlags& b) { return static_cast<uint8_t>(a) | static_cast<uint8_t>(b); }
    uint8_t operator& (const UniformFlags& a, const UniformFlags& b) { return static_cast<uint8_t>(a) & static_cast<uint8_t>(b); }
    uint8_t operator& (const uint8_t& a, const UniformFlags& b) { return a & static_cast<uint8_t>(b); }
    uint8_t operator& (const UniformFlags& a, const uint8_t& b) { return static_cast<uint8_t>(a) & b; }
}
