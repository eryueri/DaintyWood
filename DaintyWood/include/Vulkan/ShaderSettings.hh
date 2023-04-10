#pragma once

#include "pch.hh"

#define BIT(x) 1 << x
namespace  DWE {
    enum class ShaderType : uint8_t {
        Vertex, Fragment, 
        Geometry, Compute,
    };

    enum class VertexDataFlag : uint8_t {
        None = 0,
        Position = BIT(0), 
        Color = BIT(1), 
        Texcoord = BIT(2),
        Normal = BIT(3),
    };

    enum class UniformFlag : uint8_t {
        None = 0,
        ModelMatrix = BIT(0), 
        ViewMatrix = BIT(1), 
        ProjectionMatrix = BIT(2),
        ViewProjectionMatrix = BIT(3),
        ModelViewProjectionMatrix = BIT(4),
    };

    struct ShaderSettings {
        std::string root_dir = "./";
        std::optional<std::string> shader_name;
        std::optional<std::string> compiled_file_path;
        std::optional<std::string> shader_type;
        std::optional<std::string> entry_point = "main";
        std::vector<std::optional<std::string>> vertex_data_flags;
        std::vector<std::optional<std::string>> uniform_data_flags;
        std::vector<std::optional<std::string>> sampler_name_list;
    };

    uint32_t vertexDataStride(VertexDataFlag flag);

    uint8_t operator| (const VertexDataFlag& a, const VertexDataFlag& b);
    uint8_t operator| (const uint8_t& a, const VertexDataFlag& b);
    uint8_t operator| (const VertexDataFlag& a, const uint8_t& b);
    uint8_t operator& (const VertexDataFlag& a, const VertexDataFlag& b);
    uint8_t operator& (const uint8_t& a, const VertexDataFlag& b);
    uint8_t operator& (const VertexDataFlag& a, const uint8_t& b);

    void operator|= (VertexDataFlag& a, const VertexDataFlag& b);
    void operator|= (uint8_t& a, const VertexDataFlag& b);
    void operator|= (VertexDataFlag& a, const uint8_t& b);
    void operator&= (VertexDataFlag& a, const VertexDataFlag& b);
    void operator&= (uint8_t& a, const VertexDataFlag& b);
    void operator&= (VertexDataFlag& a, const uint8_t& b);

    uint8_t operator| (const UniformFlag& a, const UniformFlag& b);
    uint8_t operator| (const uint8_t& a, const UniformFlag& b);
    uint8_t operator| (const UniformFlag& a, const uint8_t& b);
    uint8_t operator& (const UniformFlag& a, const UniformFlag& b);
    uint8_t operator& (const uint8_t& a, const UniformFlag& b);
    uint8_t operator& (const UniformFlag& a, const uint8_t& b);

    void operator|= (UniformFlag& a, const UniformFlag& b);
    void operator|= (uint8_t& a, const UniformFlag& b);
    void operator|= (UniformFlag& a, const uint8_t& b);
    void operator&= (UniformFlag& a, const UniformFlag& b);
    void operator&= (uint8_t& a, const UniformFlag& b);
    void operator&= (UniformFlag& a, const uint8_t& b);
}
