#pragma once 

#include "pch.hh"
#include "Vulkan/VulkanUtils.hh"

struct GLFWwindow;

namespace DWE {
    class VulkanInstance {
    public:
        VulkanInstance() = delete;
        VulkanInstance(GLFWwindow* window);
        ~VulkanInstance();
    public:
        GLFWwindow* getGLFWwindow() const;
        VulkanUtils* getUtil() const;
        vk::PhysicalDevice getGPU() const;
        vk::Device getLogicalDevice() const;
        vk::SurfaceKHR getSurface() const;
        vk::RenderPass getRenderPass() const;
        vk::Extent2D getSwapchainExtent() const;
        vk::CommandBuffer getPrimaryCommandBuffer(uint32_t index) const;
        uint32_t getMemoryType(uint32_t filter, vk::MemoryPropertyFlags prop) const;
    public:
        uint32_t waitAvailableFrameBuffer();
        vk::CommandBuffer getSingleTimeCommandsBegin();
        void getSingleTimeCommandsEnd();
        void getRenderCommandBufferBegin(uint32_t image_index);
        void getRenderCommandBufferend(uint32_t image_index);
        void submitCommands(uint32_t image_index);
        void presentFrame();
    private:
        void createInstance();
        void createSurface();
        void pickGPU();
        void createLogicalDevice();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createCommandPools();
        void createPrimaryCommandBuffers();
        void createSecondaryCommandBuffer(uint32_t image_index, uint32_t secondary_buffer_index);
        void createFramebuffers();
        void createSyncObjects();
    private:
        void cleanInstance();
        void cleanSurface();
        void cleanLogicalDevice();
        void cleanSwapchain();
    private:
        GLFWwindow* const _glfw_window;
        vk::ClearValue _clear_color{vk::ClearColorValue{0.0f,0.0f,0.0f,1.0f}};
        const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
        uint32_t _current_frame = 0;
        uint32_t _current_image = 0;
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
        std::vector<vk::CommandPool> _command_pools;
        vk::CommandPool _single_time_command_pool = nullptr;
        std::vector<vk::CommandBuffer> _primary_command_buffers;
        std::vector<std::vector<vk::CommandBuffer>> _secondary_command_buffers;
        vk::CommandBuffer _single_time_command_buffer = nullptr;
        std::vector<vk::Framebuffer> _framebuffers;

        std::vector<vk::Semaphore> _image_available_semaphores;
        std::vector<vk::Semaphore> _render_finished_semaphores;
        std::vector<vk::Fence> _inflight_fences;
        std::vector<vk::Queue> _queues;
        std::optional<uint32_t> _graphics_queue_index, _present_queue_index;
        QueueFamilyIndices _queue_indices;
        std::unique_ptr<VulkanUtils> _util;
    };
}
