#pragma once

#include <vulkan/vulkan.hpp>

namespace DWE {
    class VulkanEntity {
    public:
        VulkanEntity();
        virtual ~VulkanEntity();

        virtual void createPipelineLayout() = 0;
        virtual void createPipeline() = 0;
        virtual void createDescriptorPool() = 0;
        virtual void createDescriptorSets() = 0;
    private:
        vk::PipelineLayout _pipeline_layout = nullptr;
        vk::Pipeline _pipeline = nullptr;

        vk::DescriptorPool _descriptor_pool = nullptr;
        std::vector<vk::DescriptorSet> _descriptor_sets;
    };
}
