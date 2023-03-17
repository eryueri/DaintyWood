#pragma once

#include <Vulkan/VulkanEntity.hh>

namespace DWE {
    class VulkanInstance;
    class VulkanTexture;
    class VulkanMesh;
    class VulkanShader;
    class VulkanRenderEntity : public VulkanEntity {
    public:
        VulkanRenderEntity();
        ~VulkanRenderEntity();
        void updateDescriptorSets();
        void writeDrawingCommands() override;
    private:
        void createPipelineLayout() override;
        void cleanPipelineLayout();

        void createPipeline() override;
        void cleanPipeline();
        void resetPipeline();

        void createDescriptorPool() override;
        void cleanDescriptorPool();
        void createDescriptorSets() override;
        void cleanDescriptorSets();

        void updateDescriptorSet() override;

    private:
        VulkanInstance* _vulkan_instance = nullptr;
        VulkanTexture* _vulkan_texture = nullptr;
        VulkanMesh* _vulkan_mesh = nullptr;
        VulkanShader* _vertex_shader = nullptr;
        VulkanShader* _frag_shader = nullptr;
    private:
        vk::PipelineLayout _pipeline_layout = nullptr;
        vk::Pipeline _graphics_pipeline = nullptr;

        vk::DescriptorPool _descriptor_pool = nullptr;
        std::vector<vk::DescriptorSet> _descriptor_sets;
    };
}
