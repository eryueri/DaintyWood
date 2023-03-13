#pragma once

#include <vulkan/vulkan.hpp>

namespace DWE {
    class VulkanInstance;
    class VulkanTexture {
    public:
        VulkanTexture();
        ~VulkanTexture();
    private:
        VulkanInstance* _instance;
    private:
        vk::Image _image;
    };
}
