#pragma once

#include <vulkan/vulkan.hpp>

#include "Vulkan/TextureSettings.hh"

namespace DWE {
    class VulkanInstance;
    struct SamplerInfo {
        std::string sampler_name;
        AddressMode address_mode;
        BorderColor border_color;
    };
    class VulkanTexture {
    public:
        VulkanTexture() = delete;
        VulkanTexture(VulkanInstance* instance, const TextureSettings& settings);
        ~VulkanTexture();

        SamplerInfo getSamplerInfo();
        vk::ImageView getImageView();
    private:
        void createTextureImage(std::string file_path);
        void createTextureImageView();
        void transitionImageLayout(
                vk::Image image,
                vk::Format format,
                vk::ImageLayout old_layout,
                vk::ImageLayout new_layout
                );
        void copyBufferToImage(vk::Buffer buffer, vk::Image image, const uint32_t& width, const uint32_t& height);
    private:
        VulkanInstance* _instance = nullptr;
    private:
        std::string _texture_name;
        std::string _sampler_name;
        TextureType _type;
        AddressMode _address_mode;
        BorderColor _border_color;
    private:
        vk::Image _image;
        vk::DeviceMemory _image_memory;
        vk::ImageView _image_view;
    };
}
