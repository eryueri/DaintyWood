#pragma once 

#include "pch.hh"

#include "Vulkan/Structs.hh"
#include "Vulkan/Utils.hh"

struct GLFWwindow;

namespace DWE {
    class VulkanInstance {
    public:
        VulkanInstance() = delete;
        VulkanInstance(GLFWwindow* window);
        ~VulkanInstance();
    public:
        GLFWwindow* getGLFWwindow() const;
        vk::PhysicalDevice getGPU() const;
        vk::Device getLogicalDevice() const;
        vk::SurfaceKHR getSurface() const;
    private:
        void createInstance();
        void cleanInstance();
        void createSurface();
        void cleanSurface();
        void pickGPU();
        void createLogicalDevice();
        void cleanLogicalDevice();
        void createSwapChain();
        void cleanSwapchain();
        void createImageViews();
        void createRenderPass();
    private:
        GLFWwindow* const _glfw_window;
    private:
        vk::Instance _instance = nullptr;
        vk::SurfaceKHR _surface = nullptr;
        vk::PhysicalDevice _gpu = nullptr;
        vk::Device _logical_device = nullptr;
        vk::SwapchainKHR _swapchain = nullptr;
        std::vector<vk::Image> _swapchain_images;
        vk::Format _swapchain_format;
        vk::Extent2D _swapchain_extent;
        std::vector<vk::ImageView> _image_views;
        vk::RenderPass _render_pass = nullptr;
    private:
        std::vector<vk::Queue> _queues;
        std::optional<uint32_t> _graphics_queue_index, _present_queue_index;
        QueueFamilyIndices _queue_indices;
        VulkanUtils _utils;
    };
}
