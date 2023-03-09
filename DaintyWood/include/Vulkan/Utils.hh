#pragma once

#include "Vulkan/Structs.hh" 

struct GLFWwindow;

namespace DWE {
    class VulkanInstance;
    class VulkanUtils {
    public:
        VulkanUtils() = delete;
        VulkanUtils(VulkanInstance* instance);

        void findQueueFamilyIndices(QueueFamilyIndices& indices);
        void querySwapChainSupport(SwapchainSupports& details);
        void setSwapchainDetails(const SwapchainSupports& supports, SwapchainDetails& details);

    private:
        const VulkanInstance* _instance;
    };

    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available_formats);
    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& available_modes);
    vk::Extent2D chooseSwapExtent(GLFWwindow* window, const vk::SurfaceCapabilitiesKHR& capabilities);
}
