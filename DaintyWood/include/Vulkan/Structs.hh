#pragma once

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
