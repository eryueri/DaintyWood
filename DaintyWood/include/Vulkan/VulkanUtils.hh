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
    private:
        vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available_formats);
        vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& available_modes);
        vk::Extent2D chooseSwapExtent(GLFWwindow* window, const vk::SurfaceCapabilitiesKHR& capabilities);
    private:
        const VulkanInstance* _instance;
    };

}
