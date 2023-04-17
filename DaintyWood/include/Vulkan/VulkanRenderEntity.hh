#pragma once

#include <Vulkan/VulkanEntity.hh>

#include "Vulkan/VulkanTexture.hh"
#include "Vulkan/VulkanMesh.hh"
#include "Vulkan/VulkanShader.hh"
#include "Vulkan/UniformData.hh"

namespace DWE {
    class VulkanInstance;
    class VulkanRenderEntity : public VulkanEntity {
    public:
        VulkanRenderEntity() = delete;
        VulkanRenderEntity(VulkanInstance* instance, EntitySettings settings);
        ~VulkanRenderEntity();
        void writeDrawingCommands(uint32_t image_index) override;
    public:
        void addTexture(VulkanTexture* texture);
        void addMesh(VulkanMesh* mesh);
        void addShader(VulkanShader* shader);
        void initialize();

        void updateUniformData(const uint32_t& image_index, const UniformData& data) override;
    private:
        void createPipelineLayout() override;

        void createPipeline() override;
        void resetPipeline();

        void createDescriptorPool() override;

        void createDescriptorSets() override;
        void updateDescriptorSets() override;

        void updateDescriptorSet() override;

        void createUniformBuffers();

        void createSamplers();

    private:
        std::vector<vk::DescriptorSet> getDescriptorSets(uint32_t image_index); // image_index is index of swapchain image which is got from waitframebuffer;

    private:
        void cleanPipelineLayout();
        void cleanPipeline();
        void cleanDescriptorPool();
        void cleanDescriptorSets();

    private:
        VulkanInstance* _vulkan_instance = nullptr;
        VulkanUtils* _utils = nullptr;
        VulkanShader* _vertex_shader = nullptr;
        VulkanShader* _frag_shader = nullptr;
    private:
        EntityType _type;
        MeshCullMode _cull_mode;
        uint8_t _vertex_data_flags = 0;
        uint8_t _uniform_data_flags = 0;
        std::vector<VulkanTexture*> _vulkan_textures;
        std::vector<VulkanMesh*> _vulkan_meshes;
        std::vector<VulkanShader*> _vulkan_shaders;
    private:
        vk::PipelineLayout _pipeline_layout = nullptr;
        vk::Pipeline _graphics_pipeline = nullptr;

        vk::DescriptorPool _descriptor_pool = nullptr;
        std::map<VulkanShader*, std::vector<vk::DescriptorSet>> _descriptor_sets;
        std::map<std::string, vk::Sampler> _samplers;
        std::map<std::string, vk::ImageView> _image_views;
        std::vector<vk::Buffer> _uniform_buffers;
        std::vector<vk::DeviceMemory> _uniform_memories;
        std::vector<void*> _memory_maps;
    };
}
