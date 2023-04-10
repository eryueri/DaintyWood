#pragma once

#include <vulkan/vulkan.hpp>

#include "Vulkan/ShaderSettings.hh"

namespace DWE {
    class VulkanShader {
    public:
        VulkanShader() = delete;
        VulkanShader(vk::Device device, const ShaderSettings& settings);
        ~VulkanShader();

        ShaderType getShaderType();
        uint8_t getVertexDataFlags();
        uint8_t getUniformDataFlags();
        vk::PipelineShaderStageCreateInfo getStageInfo();
        std::vector<vk::VertexInputBindingDescription> getVertexBindingDescriptions();
        std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDexcriptions();
        vk::DescriptorSetLayout getDescriptorSetLayout();

        uint32_t getUniformBufferCount();
        uint32_t getUniformBufferSize();
        uint32_t getSamplerCount();
        std::vector<std::string> getSamplerNameList();

        void cleanShaderModule();
    private:
        void createShaderModule();
        void createDescriptorSetLayout();

        void cleanDescriptorSetLayout();

    private:
        std::string _shader_name;
        std::string _compiled_file_path;
        std::string _entry_point;
        uint8_t _vertex_data_flags = 0;
        uint32_t _vertex_data_flag_count = 0;
        uint8_t _uniform_data_flags = 0;
        uint32_t _uniform_data_flag_count = 0;
        std::vector<std::string> _sampler_names;
        ShaderType _shader_type;
        vk::Device _device = nullptr;
        vk::DescriptorSetLayout _descriptor_set_layout = nullptr;
        vk::ShaderModule _shader_bin = nullptr;
        vk::ShaderStageFlagBits _shader_stage_flag_bits{0};
    };
}
