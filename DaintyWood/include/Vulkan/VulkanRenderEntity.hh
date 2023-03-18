#pragma once

#include <Vulkan/VulkanEntity.hh>

#include "Vulkan/VulkanTexture.hh"
#include "Vulkan/VulkanMesh.hh"
#include "Vulkan/VulkanShader.hh"

namespace DWE {
    class VulkanInstance;
    class VulkanRenderEntity : public VulkanEntity {
    public:
        VulkanRenderEntity() = delete;
        VulkanRenderEntity(EntitySettings settings);
        ~VulkanRenderEntity();
        void updateDescriptorSets();
        void writeDrawingCommands() override;
    public:
        void addTexture(VulkanTexture* texture);
        void addMesh(VulkanMesh* mesh);
        void addShader(VulkanShader* shader);
        void initialize();
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
        VulkanShader* _vertex_shader = nullptr;
        VulkanShader* _frag_shader = nullptr;
    private:
        EntityType _type;
        MeshCullMode _cull_mode;
        std::vector<VulkanTexture*> _vulkan_textures;
        std::vector<VulkanMesh*> _vulkan_meshes;
        std::vector<VulkanShader*> _vulkan_shaders;
    private:
        vk::PipelineLayout _pipeline_layout = nullptr;
        vk::Pipeline _graphics_pipeline = nullptr;

        vk::DescriptorPool _descriptor_pool = nullptr;
        std::vector<vk::DescriptorSet> _descriptor_sets;
    };
}
