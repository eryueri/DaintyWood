#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include "Vulkan/MeshSettings.hh"

struct aiNode;
struct aiMesh;
struct aiScene;

namespace DWE {
    enum class MeshCullMode : uint8_t {
        FrontFace, 
        BackFace,
        None
    };
    class VulkanMesh {
    public:
        VulkanMesh() = delete;
        VulkanMesh(MeshSettings settings);
        ~VulkanMesh();
    public:
        void setVertexDataFlags(uint8_t flags);
    private:
        void createGeometryBuffers();
        void cleanGeometryBuffers();
    private:
        void processNode(aiNode* node, const aiScene* scene);
        void processMesh(aiMesh* mesh, const aiScene* scene);
    private:
        std::string _mesh_name;
        uint8_t _vertex_data_flags = 0;
    private:
        std::vector<glm::vec3> _vertex_positions;
        std::vector<glm::vec4> _vertex_colors;
        std::vector<glm::vec2> _vertex_textures;
        std::vector<glm::vec3> _vertex_normals;
        std::vector<uint32_t> _vertex_indexs;
        std::vector<vk::Buffer> _vertex_buffers;
        std::vector<vk::DeviceMemory> _vertex_buffer_memories;
        vk::Buffer _index_buffer;
        vk::Buffer _index_buffer_memory;
    };
}
