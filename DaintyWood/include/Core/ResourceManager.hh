#pragma once

#include "pch.hh"

namespace DWE {
    struct TextureSettings;
    struct MeshSettings;
    struct ShaderSettings;
    struct EntitySettings;
    class VulkanTexture;
    class VulkanMesh;
    class VulkanShader;
    class VulkanEntity;
    class ResourceManager {
    public:
        ResourceManager();
        ResourceManager(std::string source_root_dir);
        ~ResourceManager();

    public:
        void loadFile(std::string file);

    private:
        void loadTexture(TextureSettings settings);
        void loadMesh(MeshSettings settings);
        void loadShader(ShaderSettings settings);

    private:
        std::string _source_root_dir = "./";

    private:
        std::unordered_map<std::string, VulkanTexture> _texture_map;
        std::unordered_map<std::string, VulkanMesh> _mesh_map;
        std::unordered_map<std::string, VulkanShader> _shader_map;
        std::vector<VulkanEntity> _entities;
    };
}
