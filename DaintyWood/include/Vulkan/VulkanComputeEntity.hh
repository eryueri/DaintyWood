#pragma once

#include "Vulkan/VulkanEntity.hh"

namespace DWE {
    // to be done in the far future
    class VulkanComputeEntity : public VulkanEntity {
    public:
        VulkanComputeEntity() = delete;
        VulkanComputeEntity(EntitySettings settings);
        ~VulkanComputeEntity();
    };
}
