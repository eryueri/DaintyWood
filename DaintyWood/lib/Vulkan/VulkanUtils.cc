#include "Vulkan/VulkanUtils.hh"

#include <GLFW/glfw3.h>

#include "Vulkan/VulkanInstance.hh"

#include "Vulkan/Macro.hh"
namespace DWE {
    bool QueueFamilyIndices::isComplete()
    {
        return graphics_family.has_value() && present_family.has_value();
    }

    bool QueueFamilyIndices::isDiscrete()
    {
        return graphics_family.value() != present_family.value();
    }

    std::vector<uint32_t> QueueFamilyIndices::operator()()
    {
        return std::vector<uint32_t>{graphics_family.value(), present_family.value()};
    }
}

namespace DWE {
    VulkanUtils::VulkanUtils(VulkanInstance* instance) : _instance(instance) {}
    vk::QueueFamilyProperties VulkanUtils::findQueueFamilyIndices(QueueFamilyIndices& indices)
    {
        vk::PhysicalDevice gpu = _instance->getGPU();
        std::vector<vk::QueueFamilyProperties> queue_props = gpu.getQueueFamilyProperties();
        uint32_t i = 0;
        for (vk::QueueFamilyProperties queue_prop : queue_props) {
            if (gpu.getSurfaceSupportKHR(i, _instance->getSurface())) {
                indices.present_family = i;
            }
            if (queue_prop.queueFlags & vk::QueueFlagBits::eGraphics) {
                indices.graphics_family = i;
            }
            if (indices.isComplete()) {
                return queue_prop;
            }
            ++i;
        }

        throw std::runtime_error("failed to find proper queue family...");
    }

    void VulkanUtils::querySwapChainSupport(SwapchainSupports &details)
    {
        vk::PhysicalDevice gpu = _instance->getGPU();
        vk::SurfaceKHR surface = _instance->getSurface();
        details.capabilities = gpu.getSurfaceCapabilitiesKHR(surface);
        details.formats = gpu.getSurfaceFormatsKHR(surface);
        details.present_modes = gpu.getSurfacePresentModesKHR(surface);
    }

    void VulkanUtils::setSwapchainDetails(const SwapchainSupports &supports, SwapchainDetails& details)
    {
        details.extent = chooseSwapExtent(_instance->getGLFWwindow(), supports.capabilities);
        details.surface_format = chooseSwapSurfaceFormat(supports.formats);
        details.present_mode = chooseSwapPresentMode(supports.present_modes);
    }

    vk::SurfaceFormatKHR VulkanUtils::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available_formats)
    {
        for (const auto& available_format : available_formats) {
            if (available_format.format == vk::Format::eB8G8R8A8Srgb && available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            {
                return available_format;
            }
        }
        return available_formats[0];
    }

    vk::PresentModeKHR VulkanUtils::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& available_modes)
    {
        for (const auto& available_mode : available_modes) {
            if (available_mode == vk::PresentModeKHR::eMailbox){
                return available_mode;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D VulkanUtils::chooseSwapExtent(GLFWwindow* window, const vk::SurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        width = std::clamp(static_cast<uint32_t>(width), capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        height = std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        vk::Extent2D actual_extent{
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        return actual_extent;
    }

    void VulkanUtils::allocateBufferMemory(
            vk::DeviceSize size, 
            vk::BufferUsageFlags usage, 
            vk::MemoryPropertyFlags property, 
            vk::Buffer &buffer, 
            vk::DeviceMemory &memory)
    {
        vk::Device device = _instance->getLogicalDevice();
        vk::BufferCreateInfo buffer_create_info{};
        buffer_create_info
            .setSize(size)
            .setUsage(usage)
            .setSharingMode(vk::SharingMode::eExclusive);

        buffer = device.createBuffer(buffer_create_info);
        CHECK_NULL(buffer);

        vk::MemoryRequirements memory_requirements{};
        memory_requirements = device.getBufferMemoryRequirements(buffer);

        vk::MemoryAllocateInfo memory_allocate_info{}; // TODO: memory allocation is limited under 4096, implement a better memory allocation stratage in the far future
        memory_allocate_info
            .setAllocationSize(memory_requirements.size)
            .setMemoryTypeIndex(_instance->getMemoryType(memory_requirements.memoryTypeBits, property));

        memory = device.allocateMemory(memory_allocate_info);
        CHECK_NULL(memory);
        device.bindBufferMemory(buffer, memory, 0);
    }

    void VulkanUtils::allocateImage(
            uint32_t width, 
            uint32_t height, 
            vk::Format format, 
            vk::ImageTiling tiling, 
            vk::ImageUsageFlags usage, 
            vk::MemoryPropertyFlags memory_property, 
            vk::Image &image, 
            vk::DeviceMemory &image_memory)
    {
        vk::Device device = _instance->getLogicalDevice();

        vk::ImageCreateInfo image_create_info{};
        image_create_info
            .setImageType(vk::ImageType::e2D)
            .setExtent(vk::Extent3D{width, height, 1})
            .setMipLevels(1)
            .setArrayLayers(1)
            .setFormat(format)
            .setTiling(tiling)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setUsage(usage)
            .setSamples(vk::SampleCountFlagBits::e1)
            .setSharingMode(vk::SharingMode::eExclusive);

        image = device.createImage(image_create_info);
        CHECK_NULL(image);

        vk::MemoryRequirements requirements = device.getImageMemoryRequirements(image);

        vk::MemoryAllocateInfo memory_allocate_info{};
        memory_allocate_info
            .setAllocationSize(requirements.size)
            .setMemoryTypeIndex(_instance->getMemoryType(requirements.memoryTypeBits, memory_property));

        image_memory = device.allocateMemory(memory_allocate_info);
        CHECK_NULL(image_memory);
        device.bindImageMemory(image, image_memory, 0);
    }

    void VulkanUtils::createImageView(
            const vk::Image& image, 
            vk::Format format, 
            vk::ImageView& image_view
            )
    {
        vk::Device device = _instance->getLogicalDevice();
        vk::ImageSubresourceRange range{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};

        vk::ImageViewCreateInfo create_info{};
        create_info
            .setImage(image)
            .setViewType(vk::ImageViewType::e2D)
            .setFormat(format)
            .setSubresourceRange(range);

        image_view = device.createImageView(create_info);
        CHECK_NULL(image_view);
    }

}
#include "Vulkan/UnMacro.hh"
