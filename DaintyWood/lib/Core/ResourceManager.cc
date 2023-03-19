#include "Core/ResourceManager.hh"

#include "Vulkan/VulkanTexture.hh"
#include "Vulkan/VulkanMesh.hh"
#include "Vulkan/VulkanShader.hh"
#include "Vulkan/VulkanEntity.hh"
#include "Vulkan/VulkanRenderEntity.hh"
#include "Vulkan/VulkanInstance.hh"
// #include "Vulkan/VulkanComputeEntity.hh"

namespace DWE {
    ResourceManager::ResourceManager(VulkanInstance* vulkan_instance)
        : _vulkan_instance(vulkan_instance)
    {

    }

    ResourceManager::ResourceManager(VulkanInstance* vulkan_instance, std::string source_root_dir) 
        : _vulkan_instance(vulkan_instance), 
        _source_root_dir(source_root_dir)
    {

    }

    ResourceManager::~ResourceManager()
    {

    }

    std::vector<VulkanEntity*> ResourceManager::getRenderEntities() const
    {
        return _render_entities;
    }

    void ResourceManager::loadFile(std::string file) {
        toml::table config = toml::parse_file(_source_root_dir + file);

        if (config["Textures"]) {
            if (!config["Textures"].is_array()) throw std::runtime_error("Textures sould be configured as array of tables");
            toml::array& texture_config = *config["Textures"].as_array();
            for (const toml::node& config : texture_config) {
                if (!config.is_table()) 
                    throw std::runtime_error("texture config is not proper...");
                toml::table texture = *config.as_table();
            }
        }
        if (config["Meshes"]) {
            if (!config["Meshes"].is_array()) throw std::runtime_error("Meshes sould be configured as array of tables");
            toml::array& mesh_config = *config["Meshes"].as_array();
            for (const toml::node& config : mesh_config) {
                if (!config.is_table()) 
                    throw std::runtime_error("mesh config is not proper...");
                toml::table mesh = *config.as_table();
                loadMesh(mesh);
            }
        }
        if (config["Shaders"]) {
            if (!config["Shaders"].is_array()) throw std::runtime_error("Shaders sould be configured as array of tables");
            toml::array& shader_config = *config["Shaders"].as_array();
            for (const toml::node& config : shader_config) {
                if (!config.is_table()) 
                    throw std::runtime_error("shader config is not proper...");
                toml::table shader = *config.as_table();
                loadShader(shader);
            }
        }
        if (config["Entities"]) {
            if (!config["Entities"].is_array()) throw std::runtime_error("Entities sould be configured as array of tables");
            toml::array& entity_config = *config["Entities"].as_array();
            for (const toml::node& config : entity_config) {
                if (!config.is_table()) 
                    throw std::runtime_error("entity config is not proper...");
                toml::table entity = *config.as_table();
                loadEntity(entity);
            }
        }
    }

    void ResourceManager::loadTexture(toml::table texture)
    {

    }

    void ResourceManager::loadMesh(toml::table mesh) 
    {
        MeshSettings settings{};
        settings.root_dir = _source_root_dir;
        settings.mesh_name = mesh["name"].value<std::string>();
        settings.file_path = mesh["filePath"].value<std::string>();
        settings.cull_mode = mesh["cullMode"].value<std::string>();

        VulkanMesh* newMesh = new VulkanMesh(_vulkan_instance, settings);
        if (_mesh_map.count(settings.mesh_name.value())) {
            throw std::runtime_error("mesh already existed: " + settings.mesh_name.value() + "\nuse another name if do want this mesh loaded");
        }
        _mesh_map.insert({settings.mesh_name.value(), newMesh});
    }

    void ResourceManager::loadShader(toml::table shader)
    {
        ShaderSettings settings{};
        settings.root_dir = _source_root_dir;
        settings.shader_name = shader["name"].value<std::string>();
        settings.compiled_file_path = shader["fileName"].value<std::string>();
        settings.shader_type = shader["shaderType"].value<std::string>();
        settings.entry_point = shader["entryPoint"].value<std::string>();

        if (shader["vertexInfo"]["vertexDataFlags"].is_array()) {
            toml::array vertex_flags = *shader["vertexInfo"]["vertexDataFlags"].as_array();
            for (const toml::node& flags : vertex_flags) {
                settings.vertex_data_flags.push_back(flags.value<std::string>());
            }
        }
        if (shader["uniformInfo"]["uniformFlags"].is_array()) {
            toml::array uniform_flags = *shader["uniformInfo"]["uniformFlags"].as_array();
            for (const toml::node& flags : uniform_flags) {
                settings.uniform_data_flags.push_back(flags.value<std::string>());
            }
        }
        if (shader["samplernamelist"].is_array()) {
            toml::array sampler_list = *shader["samplernamelist"].as_array();
            for (const toml::node& sampler : sampler_list) {
                settings.sampler_name_list.push_back(sampler.value<std::string>());
            }
        }

        VulkanShader* newShader = new VulkanShader(_vulkan_instance->getLogicalDevice(), settings);
        if (_mesh_map.count(settings.shader_name.value())) {
            throw std::runtime_error("shader already existed: " + settings.shader_name.value() + "\nuse another name if do want this shader loaded");
        }
        _shader_map.insert({settings.shader_name.value(), newShader});
    }

    void ResourceManager::loadEntity(toml::table entity)
    {
        EntitySettings settings{};
        settings.entity_type = entity["entityType"].value<std::string>();
        settings.cull_mode = entity["cullMode"].value<std::string>();
        EntityResourceData resource_data{};
        if (entity["meshNames"].is_array()) {
            toml::array mesh_names = *entity["meshNames"].as_array();
            for (const toml::node& mesh_name : mesh_names) {
                resource_data.mesh_names.push_back(mesh_name.value<std::string>());
            }
        }
        if (entity["textureNames"].is_array()) {
            toml::array texture_names = *entity["textureNames"].as_array();
            for (const toml::node& texture_name : texture_names) {
                resource_data.texture_names.push_back(texture_name.value<std::string>());
            }
        }
        if (entity["shaderNames"].is_array()) {
            toml::array shader_names = *entity["shaderNames"].as_array();
            for (const toml::node& shader_name : shader_names) {
                resource_data.shader_names.push_back(shader_name.value<std::string>());
            }
        }

        // ComputeEntity is future work, i'll not implement it for now
        //
        // if (settings.entity_type.value() == "RenderEntity") {
        //     VulkanEntity* entity = new VulkanRenderEntity(settings);
        //     _render_entities.push_back(entity);
        // } else if (settings.entity_type.value() == "ComputeEntity") {
        //     VulkanEntity* entity = new VulkanComputeEntity(settings);
        //     _compute_entities.push_back(entity);
        // } else {
        //     throw std::runtime_error("unknown entity type: " + settings.entity_type.value());
        // }

        VulkanRenderEntity* newEntity = new VulkanRenderEntity(_vulkan_instance, settings);
        for (const std::optional<std::string>& mesh_name : resource_data.mesh_names) {
            if (mesh_name.has_value()) {
                newEntity->addMesh(_mesh_map.at(mesh_name.value()));
            }
        }
        for (const std::optional<std::string>& texture_name : resource_data.texture_names) {
            if (texture_name.has_value()) {
                newEntity->addTexture(_texture_map.at(texture_name.value()));
            }
        }
        for (const std::optional<std::string>& shader_name : resource_data.shader_names) {
            if (shader_name.has_value()) {
                newEntity->addShader(_shader_map.at(shader_name.value()));
            }
        }
        newEntity->initialize();
        _render_entities.push_back(newEntity);
    }
        
}
