#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include "Vulkan/MeshSettings.hh"

namespace DWE {
    enum class MeshCullMode : uint8_t {
        FrontFace, 
        BackFace,
        None
    };
    class VulkanMesh {
    public:
        VulkanMesh();
        ~VulkanMesh();
    public:
        MeshCullMode getCullMode();
    private:
        std::string _name;
        MeshCullMode _cull_mode;
    private:
        std::vector<glm::vec3> _vertex_positions;
        std::vector<glm::vec2> _vertex_textures;
        std::vector<glm::vec3> _vertex_normals;
        std::vector<uint32_t> _vertex_indexs;
        std::vector<vk::Buffer> _vertex_buffers;
        std::vector<vk::DeviceMemory> _vertex_buffer_memories;
        vk::Buffer _index_buffer;
        vk::Buffer _index_buffer_memory;
    };
}
