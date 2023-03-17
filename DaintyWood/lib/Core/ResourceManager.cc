#include "Core/ResourceManager.hh"

#include "Vulkan/VulkanTexture.hh"
#include "Vulkan/VulkanMesh.hh"
#include "Vulkan/VulkanShader.hh"
#include "Vulkan/VulkanEntity.hh"

#include <toml++/toml.h>

namespace DWE {
    ResourceManager::ResourceManager() {

    }

    ResourceManager::ResourceManager(std::string source_root_dir) 
        : _source_root_dir(source_root_dir)
    {

    }

    ResourceManager::~ResourceManager()
    {

    }

    void ResourceManager::loadFile(std::string file) {
        toml::table config = toml::parse_file(file);

        toml::array& texture_config = *config["Textures"].as_array();
        toml::array& mesh_config = *config["Meshes"].as_array();
        toml::array& shader_config = *config["Shaders"].as_array();
        toml::array& entity_config = *config["Entities"].as_array();

        if (!texture_config.empty()) {
            for (const toml::node& config : texture_config) {
                if (!config.is_table()) 
                    throw std::runtime_error("texture config is not proper...");
                toml::table texture = *config.as_table();
            }
        }

        if (!mesh_config.empty()) {
            for (const toml::node& config : mesh_config) {
                if (!config.is_table()) 
                    throw std::runtime_error("mesh config is not proper...");
                toml::table mesh = *config.as_table();
                MeshSettings settings{};
                settings.name = mesh["name"].value<std::string>();
                settings.file_path = mesh["filePath"].value<std::string>();
                loadMesh(settings);
            }
        }

        if (!shader_config.empty()) {
            for (const toml::node& config : shader_config) {
                if (!config.is_table()) 
                    throw std::runtime_error("shader config is not proper...");
                toml::table shader = *config.as_table();
                ShaderSettings settings{};
                settings.shader_name = shader["name"].value<std::string>();
            }
        }

        if (!entity_config.empty()) {
            for (const toml::node& config : entity_config) {
                if (!config.is_table()) 
                    throw std::runtime_error("entity config is not proper...");
                toml::table entity = *config.as_table();
                
            }
        }

    }

    void ResourceManager::loadTexture(TextureSettings settings) {

    }

    void ResourceManager::loadMesh(MeshSettings settings) 
    {

    }

    void ResourceManager::loadShader(ShaderSettings settings)
    {

    }
        
}
