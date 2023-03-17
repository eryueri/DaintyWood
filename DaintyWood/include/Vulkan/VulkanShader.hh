#pragma once

#include <vulkan/vulkan.hpp>

#include "Vulkan/ShaderSettings.hh"

namespace DWE {
    class VulkanShader {
    public:
        VulkanShader() = delete;
        VulkanShader(ShaderSettings ShaderSettings);
        ~VulkanShader();

        vk::PipelineShaderStageCreateInfo getStageInfo();
        std::vector<vk::VertexInputBindingDescription> getVertexBindingDescriptions();
        std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDexcriptions();
        vk::DescriptorSetLayout getDescriptorSetLayout();

        void createShaderModule();
        void cleanShaderModule();

    private:
        ShaderSettings _settings;
        vk::ShaderModule _shader_bin;
    };
}
