#pragma once

namespace DWE {
    class VulkanTexture;
    class VulkanMesh;
    class VulkanRenderEntity {
    public:
        void updateDescriptorSets();
    private:
        void createGraphicsPipelineLayout();
        void cleanGraphicsPipelineLayout();

        void createGraphicsPipeline();
        void cleanGraphicsPipeline();
        void resetGraphicsPipeline();

        void createDescriptorPool(); 
        void cleanDescriptorPool();
        void createDescriptorSets();
        void cleanDescriptorSets();

        void updateDescriptorSet();

    private:
        VulkanTexture* _vulkan_texture;
        VulkanMesh* _vulkan_mesh;
    };
}
