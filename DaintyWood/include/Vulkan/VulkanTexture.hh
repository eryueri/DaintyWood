#pragma once

#include <vulkan/vulkan.hpp>

#include "Vulkan/TextureSettings.hh"

namespace DWE {
    class VulkanInstance;
    class VulkanTexture {
    public:
        VulkanTexture();
        ~VulkanTexture();

        void writeDrawingCommands(uint32_t image_index);
    private:
        VulkanInstance* _instance;
    private:
        std::string _texture_name;
        vk::Image _image;
    };
}
