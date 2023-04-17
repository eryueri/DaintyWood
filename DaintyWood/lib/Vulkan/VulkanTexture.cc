#include "Vulkan/VulkanTexture.hh"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Vulkan/VulkanInstance.hh"

namespace DWE {
    VulkanTexture::VulkanTexture(VulkanInstance* instance, const TextureSettings& settings)
        : _instance(instance), 
        _texture_name(settings.texture_name.value()), 
        _sampler_name(settings.sampler_name.value())
    {
        static const std::map<std::string, TextureType> texture_types = {
            {"ImageFile", TextureType::ImageFile},
        };

        static const std::map<std::string, AddressMode> address_mode = {
            {"Repeat", AddressMode::Repeat}, 
            {"MirroredRepeat", AddressMode::MirroredRepeat},
            {"ClampToEdge", AddressMode::ClampToEdge},
            {"ClampToBorder", AddressMode::ClampToBorder},
            {"MirrorClampToEdge", AddressMode::MirrorClampToEdge},
        };

        static const std::map<std::string, BorderColor> border_color = {
            {"IntOpaqueBlack", BorderColor::IntOpaqueBlack}, {"FloatOpaqueBlack", BorderColor::FloatOpaqueBlack}, 
            {"IntTransparentBlack", BorderColor::IntTransparentBlack}, {"FloatTransparentBlack", BorderColor::FloatTransparentBlack}, 
            {"IntOpaqueWhite", BorderColor::IntOpaqueWhite}, {"FloatOpaqueWhite", BorderColor::FloatOpaqueWhite}, 
        };

        _type = texture_types.at(settings.texture_type.value());
        _address_mode = address_mode.at(settings.address_mode.value());
        _border_color = border_color.at(settings.border_color.value());

        createTextureImage(settings.root_dir + settings.file_path.value());
        createTextureImageView();
    }

    VulkanTexture::~VulkanTexture()
    {
        
    }

    SamplerInfo VulkanTexture::getSamplerInfo()
    {
        SamplerInfo info{};

        info.sampler_name = _sampler_name;
        info.address_mode = _address_mode;
        info.border_color = _border_color;

        return info;
    }

    vk::ImageView VulkanTexture::getImageView()
    {
        return _image_view;
    }

    void VulkanTexture::createTextureImage(std::string file_path)
    {
        int tex_width, tex_height, tex_channels;
        stbi_uc* pixels = stbi_load(file_path.c_str(), &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha);

        if (!pixels) {
            throw std::runtime_error("failed to load texture file: " + file_path + " dose not exist...");
        }

        vk::DeviceSize image_size = tex_width * tex_height * 4 /*tex_channels in this case*/ ;

        VulkanUtils* utils = _instance->getUtil();
        vk::Device device = _instance->getLogicalDevice();

        vk::Buffer staging_buffer{};
        vk::DeviceMemory staging_memory{};
        utils->allocateBufferMemory(
                image_size, 
                vk::BufferUsageFlagBits::eTransferSrc, 
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, 
                staging_buffer, 
                staging_memory);

        void* data = device.mapMemory(
                staging_memory, 
                0, 
                image_size, 
                vk::MemoryMapFlags(0));
        memcpy(data, pixels, image_size);
        device.unmapMemory(staging_memory);

        stbi_image_free(pixels);

        utils->allocateImage(
                tex_width, 
                tex_height, 
                vk::Format::eR8G8B8A8Srgb, 
                vk::ImageTiling::eOptimal, 
                vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, 
                vk::MemoryPropertyFlagBits::eDeviceLocal, 
                _image, 
                _image_memory);

        utils->transitionImageLayout(
                _image, 
                vk::Format::eR8G8B8A8Srgb, 
                vk::ImageLayout::eUndefined, 
                vk::ImageLayout::eTransferDstOptimal
                );
        copyBufferToImage(staging_buffer, _image, tex_width, tex_height);
        utils->transitionImageLayout(
                _image, 
                vk::Format::eR8G8B8A8Srgb, 
                vk::ImageLayout::eTransferDstOptimal, 
                vk::ImageLayout::eShaderReadOnlyOptimal
                );

        device.freeMemory(staging_memory);
        device.destroyBuffer(staging_buffer);
    }

    void VulkanTexture::createTextureImageView()
    {
        _instance->getUtil()->createImageView(_image, vk::Format::eR8G8B8A8Srgb, _image_view, vk::ImageAspectFlagBits::eColor);
    }

    void VulkanTexture::copyBufferToImage(vk::Buffer buffer, vk::Image image, const uint32_t& width, const uint32_t& height)
    {
        vk::ImageSubresourceLayers layers{vk::ImageAspectFlagBits::eColor, 0, 0, 1};

        vk::BufferImageCopy copy_region{};
        copy_region
            .setBufferOffset(0)
            .setBufferRowLength(0)
            .setBufferImageHeight(0)
            .setImageSubresource(layers)
            .setImageOffset({0, 0, 0})
            .setImageExtent({width, height, 1});

        vk::CommandBuffer command_buffer = _instance->getSingleTimeCommandsBegin();
        command_buffer.copyBufferToImage(
                buffer, 
                image, 
                vk::ImageLayout::eTransferDstOptimal, 
                copy_region);
        _instance->getSingleTimeCommandsEnd();
    }
}
