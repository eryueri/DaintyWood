#include "Vulkan/VulkanMesh.hh"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Vulkan/ShaderSettings.hh"

#include "Vulkan/Macro.hh"
namespace DWE {
    VulkanMesh::VulkanMesh(MeshSettings settings) 
        : _mesh_name(settings.mesh_name.value())
    {
        Assimp::Importer importer{};
        const aiScene* scene = importer.ReadFile(
                settings.root_dir + settings.file_path.value(), 
                aiProcess_Triangulate | 
                aiProcess_FlipUVs);

        processNode(scene->mRootNode, scene);
    }

    void VulkanMesh::processNode(aiNode* node, const aiScene* scene) 
    {
        for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        for (uint32_t i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    void VulkanMesh::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
            _vertex_positions.push_back({mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
            _vertex_normals.push_back({mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z});
            if (mesh->mTextureCoords[0])
                _vertex_texcoords.push_back({mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
            else
                _vertex_texcoords.push_back({0.0f, 0.0f});
            if (mesh->mColors[0])
                _vertex_colors.push_back({mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a});
            else 
                _vertex_colors.push_back({1.0f, 1.0f, 1.0f, 1.0f});
        }

        for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
            aiFace& face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; ++j) {
                _vertex_indexs.push_back(face.mIndices[j]);
            }
        }

        // maybe in the near future we'll use material, but not today
    }

    void VulkanMesh::applyDrawingCommands(uint8_t vertex_data_flags)
    {
        vk::CommandBuffer command_buffer = _instance->getPrimaryCommandBuffer(0);

        auto buffers = getVertexDataBuffers(vertex_data_flags);
        std::vector<vk::DeviceSize> offsets{buffers.size(), 0};

        command_buffer.bindVertexBuffers(0, buffers.size(), buffers.data(), offsets.data());
        command_buffer.bindIndexBuffer(_index_buffer, 0, vk::IndexType::eUint32);
        command_buffer.drawIndexed(static_cast<uint32_t>(_vertex_indexs.size()), 1, 0, 0, 0);
    }

    std::vector<vk::Buffer> VulkanMesh::getVertexDataBuffers(uint8_t vertex_data_flags)
    {
        std::vector<vk::Buffer> buffers;
        if (!vertex_data_flags) return buffers;

        if (vertex_data_flags & VertexDataFlag::Position) {
            buffers.push_back(_buffers_map.at(VertexDataFlag::Position));
        }

        if (vertex_data_flags & VertexDataFlag::Color) {
            buffers.push_back(_buffers_map.at(VertexDataFlag::Color));
        }

        if (vertex_data_flags & VertexDataFlag::Texcoord) {
            buffers.push_back(_buffers_map.at(VertexDataFlag::Texcoord));
        }

        if (vertex_data_flags & VertexDataFlag::Normal) {
            buffers.push_back(_buffers_map.at(VertexDataFlag::Normal));
        }

        return buffers;
    }

    void VulkanMesh::createGeometryBuffers()
    {
        if (!_vertex_positions.empty()) {
            createVertexBuffer(VertexDataFlag::Position);
        }

        if (!_vertex_colors.empty()) {
            createVertexBuffer(VertexDataFlag::Color);
        }

        if (!_vertex_texcoords.empty()) {
            createVertexBuffer(VertexDataFlag::Texcoord);
        }

        if (!_vertex_normals.empty()) {
            createVertexBuffer(VertexDataFlag::Normal);
        }

        if (!_vertex_indexs.empty()) {
            createIndexBuffer();
        }
    }

    void VulkanMesh::createVertexBuffer(VertexDataFlag flag)
    {
        vk::Device device = _instance->getLogicalDevice();
        vk::DeviceSize size;
        void* src_data;

        switch (flag) {
        case VertexDataFlag::None: return;
        case VertexDataFlag::Position: {
            size = sizeof(glm::vec3) * _vertex_positions.size();
            src_data = (void*)_vertex_positions.data();
        } break;
        case VertexDataFlag::Color: {
            size = sizeof(glm::vec4) * _vertex_colors.size();
            src_data = (void*)_vertex_colors.data();
        } break;
        case VertexDataFlag::Texcoord: {
            size = sizeof(glm::vec2) * _vertex_texcoords.size();
            src_data = (void*)_vertex_texcoords.data();
        } break;
        case VertexDataFlag::Normal: {
            size = sizeof(glm::vec3) * _vertex_normals.size();
            src_data = (void*)_vertex_normals.data();
        } break;
        default: return;
        }

        vk::Buffer staging_buffer{};
        vk::DeviceMemory staging_memory{};
        createBufferMemory(
                size, 
                vk::BufferUsageFlagBits::eTransferSrc, 
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, 
                staging_buffer, 
                staging_memory);

        vk::Buffer buffer{};
        vk::DeviceMemory memory{};

        createBufferMemory(
                size, 
                vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, 
                vk::MemoryPropertyFlagBits::eDeviceLocal, 
                buffer, 
                memory);

        void* data = device.mapMemory(staging_memory, 0, size, vk::MemoryMapFlags(0));
        memcpy(data, src_data, size);
        device.unmapMemory(staging_memory);

        device.freeMemory(staging_memory);
        device.destroyBuffer(staging_buffer);

        _buffers_map.insert({flag, buffer});
        _memories_map.insert({flag, memory});
    }

    void VulkanMesh::createIndexBuffer()
    {
        vk::Device device = _instance->getLogicalDevice();
        vk::DeviceSize size = sizeof(uint32_t) * _vertex_indexs.size();
        void* src_data = (void*)_vertex_indexs.data();

        vk::Buffer staging_buffer{};
        vk::DeviceMemory staging_memory{};
        createBufferMemory(
                size, 
                vk::BufferUsageFlagBits::eTransferSrc, 
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, 
                staging_buffer, 
                staging_memory);
        createBufferMemory(
                size, 
                vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, 
                vk::MemoryPropertyFlagBits::eDeviceLocal, 
                _index_buffer, 
                _index_buffer_memory);

        void* data = device.mapMemory(staging_memory, 0, size, vk::MemoryMapFlags(0));
        memcpy(data, src_data, size);
        device.unmapMemory(staging_memory);

        device.freeMemory(staging_memory);
        device.destroyBuffer(staging_buffer);
    }

    void VulkanMesh::createBufferMemory(
            vk::DeviceSize size, 
            vk::BufferUsageFlags usage, 
            vk::MemoryPropertyFlags property, 
            vk::Buffer& buffer, 
            vk::DeviceMemory& memory)
    {
        vk::Device device = _instance->getLogicalDevice();
        vk::BufferCreateInfo buffer_create_info{};
        buffer_create_info
            .setSize(size)
            .setUsage(usage)
            .setSharingMode(vk::SharingMode::eExclusive);

        buffer = device.createBuffer(buffer_create_info);
        CHECK_NULL(buffer);

        vk::MemoryRequirements memory_requirements{};
        memory_requirements = device.getBufferMemoryRequirements(buffer);

        vk::MemoryAllocateInfo memory_allocate_info{}; // TODO: memory allocation is limited under 4096, implement a better memory allocation stratage in the far future
        memory_allocate_info
            .setAllocationSize(size)
            .setMemoryTypeIndex(_instance->getMemoryType(memory_requirements.memoryTypeBits, property));

        memory = device.allocateMemory(memory_allocate_info);
        CHECK_NULL(memory);
        device.bindBufferMemory(buffer, memory, 0);
    }

    void VulkanMesh::copyBuffer(vk::Buffer src_buffer, vk::Buffer dst_buffer, vk::DeviceSize size)
    {
        vk::BufferCopy copy_region{};
        copy_region
            .setSrcOffset(0)
            .setDstOffset(0)
            .setSize(size);

        vk::CommandBuffer command_buffer = _instance->getSingleTimeCommandsBegin();

        command_buffer.copyBuffer(src_buffer, dst_buffer, copy_region);

        _instance->getSingleTimeCommandsEnd();
    }
}
#include "Vulkan/UnMacro.hh"
