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
        uint32_t getSwapchainSize() const;
        uint32_t getMemoryType(uint32_t filter, vk::MemoryPropertyFlags prop) const;
        VulkanUtils* getVulkanUtils() const;
    public:
        uint32_t waitAvailableFrameBuffer();
        vk::CommandBuffer getSingleTimeCommandsBegin();
        void getSingleTimeCommandsEnd();
        vk::CommandBuffer getRenderCommandBuffer(uint32_t image_index) const;
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
        void createDepthResources();
    private:
        void cleanInstance();
        void cleanSurface();
        void cleanLogicalDevice();
        void cleanSwapchain();
        void cleanImageViews();
        void cleanRenderPass();
        void cleanCommandPools();
        void cleanPrimaryCommandBuffers();
        void cleanSecondaryCommandBuffers();
        void cleanFrameBuffers();
        void cleanSyncObjects();
        void cleanDepthResources();
    private:
        GLFWwindow* const _glfw_window;
        std::array<vk::ClearValue, 2> _clear_values;
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

        vk::Image _depth_image = nullptr;
        vk::DeviceMemory _depth_memory = nullptr;
        vk::ImageView _depth_image_view = nullptr;

        std::vector<vk::Semaphore> _image_available_semaphores;
        std::vector<vk::Semaphore> _render_finished_semaphores;
        std::vector<vk::Fence> _inflight_fences;
        std::vector<vk::Queue> _queues;
        std::optional<uint32_t> _graphics_queue_index, _present_queue_index;
        QueueFamilyIndices _queue_indices;
        std::unique_ptr<VulkanUtils> _util;
    };
}
