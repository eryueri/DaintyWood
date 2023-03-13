#include "Vulkan/Utils.hh"

#include <GLFW/glfw3.h>

#include "Vulkan/VulkanInstance.hh"

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
    void VulkanUtils::findQueueFamilyIndices(QueueFamilyIndices& indices)
    {
        vk::PhysicalDevice gpu = _instance->getGPU();
        std::vector<vk::QueueFamilyProperties> queue_props = gpu.getQueueFamilyProperties();
        uint32_t i = 0;
        for (vk::QueueFamilyProperties queue_prop : queue_props) {
            if (queue_prop.queueFlags & vk::QueueFlagBits::eGraphics) {
                indices.graphics_family = i;
            }
            if (gpu.getSurfaceSupportKHR(i, _instance->getSurface())) {
                indices.present_family = i;
            }
            if (indices.isComplete()) {
                break;
            }
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

    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available_formats)
    {
        for (const auto& available_format : available_formats) {
            if (available_format.format == vk::Format::eB8G8R8A8Srgb && available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            {
                return available_format;
            }
        }
        return available_formats[0];
    }

    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& available_modes)
    {
        for (const auto& available_mode : available_modes) {
            if (available_mode == vk::PresentModeKHR::eMailbox){
                return available_mode;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D chooseSwapExtent(GLFWwindow* window, const vk::SurfaceCapabilitiesKHR& capabilities)
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

}
