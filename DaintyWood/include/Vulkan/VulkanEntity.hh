#pragma once

#include <vulkan/vulkan.hpp>

namespace DWE {
    class VulkanEntity {
    public:
        VulkanEntity();
        virtual ~VulkanEntity();

        virtual void writeDrawingCommands() = 0;
    private:
        virtual void createPipelineLayout() = 0;
        virtual void createPipeline() = 0;
        virtual void createDescriptorPool() = 0;
        virtual void createDescriptorSets() = 0;
        virtual void updateDescriptorSet() = 0;
    };
}
