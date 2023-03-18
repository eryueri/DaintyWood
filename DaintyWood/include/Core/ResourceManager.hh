#pragma once

#include "pch.hh"

#include <toml++/toml.h>

namespace DWE {
    struct TextureSettings;
    struct MeshSettings;
    struct ShaderSettings;
    struct EntitySettings;
    class VulkanTexture;
    class VulkanMesh;
    class VulkanShader;
    class VulkanEntity;
    class VulkanInstance;
    class ResourceManager {
    public:
        ResourceManager(VulkanInstance* vulkan_instance);
        ResourceManager(VulkanInstance* vulkan_instance, std::string source_root_dir);
        ~ResourceManager();

    public:
        void loadFile(std::string file);

        std::vector<VulkanEntity*> getRenderEntities() const;

    private:
        void loadTexture(toml::table texture);
        void loadMesh(toml::table mesh);
        void loadShader(toml::table shader);
        void loadEntity(toml::table entity);

    private:
        std::string _source_root_dir = "./";
        VulkanInstance* _vulkan_instance = nullptr;

    private:
        std::unordered_map<std::string, VulkanTexture*> _texture_map;
        std::unordered_map<std::string, VulkanMesh*> _mesh_map;
        std::unordered_map<std::string, VulkanShader*> _shader_map;
        std::vector<VulkanEntity*> _render_entities;
        std::vector<VulkanEntity*> _compute_entities;
    };
}
