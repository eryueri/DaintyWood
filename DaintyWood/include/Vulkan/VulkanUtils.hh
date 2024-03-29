#pragma once

struct GLFWwindow;

#include <vulkan/vulkan.hpp>

namespace DWE {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool isComplete();
        bool isDiscrete();

        std::vector<uint32_t> operator()();
    };

    struct SwapchainSupports {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> present_modes;
    };

    struct SwapchainDetails {
        vk::Extent2D extent;
        vk::SurfaceFormatKHR surface_format;
        vk::PresentModeKHR present_mode;
    };

}

namespace DWE {
    class VulkanInstance;
    class VulkanUtils {
    public:
        VulkanUtils() = delete;
        VulkanUtils(VulkanInstance* instance);

        vk::QueueFamilyProperties findQueueFamilyIndices(QueueFamilyIndices& indices);
        void querySwapChainSupport(SwapchainSupports& details);
        void setSwapchainDetails(const SwapchainSupports& supports, SwapchainDetails& details);

        void allocateBufferMemory(
                vk::DeviceSize size, 
                vk::BufferUsageFlags usage, 
                vk::MemoryPropertyFlags property, 
                vk::Buffer& buffer, 
                vk::DeviceMemory& memory);

        void allocateImage(
                uint32_t width, 
                uint32_t height, 
                vk::Format format, 
                vk::ImageTiling tiling, 
                vk::ImageUsageFlags usage, 
                vk::MemoryPropertyFlags memory_property, 
                vk::Image& image, 
                vk::DeviceMemory& image_memory);

        void createImageView(
                const vk::Image& image, 
                vk::Format format, 
                vk::ImageView& image_view, 
                vk::ImageAspectFlags aspect_flags
                );

        void transitionImageLayout(
                vk::Image image,
                vk::Format format,
                vk::ImageLayout old_layout,
                vk::ImageLayout new_layout
                );

        vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);

    private:
        vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available_formats);
        vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& available_modes);
        vk::Extent2D chooseSwapExtent(GLFWwindow* window, const vk::SurfaceCapabilitiesKHR& capabilities);
    private:
        VulkanInstance* _instance;
    };

}
