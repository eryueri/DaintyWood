#pragma once

#include "pch.hh"

#define BIT(x) 1 << x
namespace  DWE {
    enum class ShaderType : uint8_t {
        Vertex, Index, 
        Geometry, Compute,
    };

    enum class VertexDataFlags : uint8_t {
        None = 0,
        Position = BIT(1), 
        Color = BIT(2), 
        Texcord = BIT(3),
        Normal = BIT(4),
    };

    struct VertexInfo {
        uint8_t vertex_data_flags;
        uint32_t position_size = 3;
        uint32_t colors_size = 3;
    };

    enum class UniformFlags : uint8_t {
        None = 0,
        ModelMatrix = BIT(1), 
        ViewMatrix = BIT(2), 
        ProjectionMatrix = BIT(3)
    };

    struct UniformInfo {
        uint8_t uninform_flags;
        uint32_t uniform_index;
    };

    struct ShaderSettings {
        std::string shader_name;
        std::filesystem::path file_path;
        ShaderType shader_type;
        VertexInfo vertex_info;
        UniformInfo uniform_info;
    };

    uint8_t operator| (const VertexDataFlags& a, const VertexDataFlags& b);
    uint8_t operator& (const VertexDataFlags& a, const VertexDataFlags& b);
    uint8_t operator& (const uint8_t& a, const VertexDataFlags& b);
    uint8_t operator& (const VertexDataFlags& a, const uint8_t& b);
    uint8_t operator| (const UniformFlags& a, const UniformFlags& b);
    uint8_t operator& (const UniformFlags& a, const UniformFlags& b);
    uint8_t operator& (const uint8_t& a, const UniformFlags& b);
    uint8_t operator& (const UniformFlags& a, const uint8_t& b);
}
