#pragma once

#include <vulkan/vulkan.hpp>

#include "Vulkan/TextureSettings.hh"

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
