#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include "Vulkan/VulkanInstance.hh"
#include "Vulkan/MeshSettings.hh"
#include "Vulkan/ShaderSettings.hh"

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
        void applyDrawingCommands(uint8_t vertex_data_flags);
    private:
        void createVertexBuffer(VertexDataFlag flag);
        void createIndexBuffer();
        void createBufferMemory(
                vk::DeviceSize size, 
                vk::BufferUsageFlags usage, 
                vk::MemoryPropertyFlags property, 
                vk::Buffer& buffer, 
                vk::DeviceMemory& memory);
        void copyBuffer(vk::Buffer src_buffer, vk::Buffer dst_buffer, vk::DeviceSize size);
        void createGeometryBuffers();
        void cleanGeometryBuffers();

        std::vector<vk::Buffer> getVertexDataBuffers(uint8_t vertex_data_flags);
    private:
        void processNode(aiNode* node, const aiScene* scene);
        void processMesh(aiMesh* mesh, const aiScene* scene);
    private:
        std::string _mesh_name;
        VulkanInstance* _instance;
    private:
        std::vector<glm::vec3> _vertex_positions;
        std::vector<glm::vec4> _vertex_colors;
        std::vector<glm::vec2> _vertex_texcoords;
        std::vector<glm::vec3> _vertex_normals;
        std::vector<uint32_t> _vertex_indexs;
        std::map<VertexDataFlag, vk::Buffer> _buffers_map;
        std::map<VertexDataFlag, vk::DeviceMemory> _memories_map;
        vk::Buffer _index_buffer;
        vk::DeviceMemory _index_buffer_memory;
    };
}
