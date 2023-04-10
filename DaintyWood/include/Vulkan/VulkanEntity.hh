#pragma once

#include "Vulkan/EntitySettings.hh"

#include <vulkan/vulkan.hpp>

namespace DWE {
    class VulkanEntity {
    public:
        virtual void writeDrawingCommands(uint32_t image_index) = 0;
    private:
        virtual void createPipelineLayout() = 0;
        virtual void createPipeline() = 0;
        virtual void createDescriptorPool() = 0;
        virtual void createDescriptorSets() = 0;
        virtual void updateDescriptorSets() = 0;
        virtual void updateDescriptorSet() = 0;
    };
}
