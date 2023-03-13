#include "Vulkan/VulkanInstance.hh"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Vulkan/Utils.hh"

#include "Vulkan/Macro.hh"
namespace DWE {
    VulkanInstance::VulkanInstance(GLFWwindow* window) 
        : 
            _glfw_window(window),
            _utils(this)
    {

    }

    VulkanInstance::~VulkanInstance()
    {
        
    }

    void VulkanInstance::createInstance()
    {
        vk::ApplicationInfo app_info;
        app_info.setPApplicationName("DWE-VK_INSTANCE")
                .setApiVersion(VK_MAKE_VERSION(1, 0, 0))
                .setPEngineName("No Engine")
                .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
                .setApiVersion(VK_API_VERSION_1_0);

        uint32_t glfw_extension_count;
        const char** glfw_extensions;

        glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

        vk::InstanceCreateInfo create_info{};
        create_info
            .setPApplicationInfo(&app_info)
            .setEnabledExtensionCount(glfw_extension_count)
            .setPpEnabledExtensionNames(glfw_extensions)
            .setEnabledLayerCount(0);

#ifdef DWE_DEBUG
        std::vector<const char*> validation_layers = {"VK_LAYER_KHRONOS_validation"};
        create_info.setPEnabledLayerNames(validation_layers)
                   .setEnabledLayerCount((uint32_t)validation_layers.size());
#endif
        _instance = vk::createInstance(create_info);

        CHECK_NULL(_instance);
    }

    void VulkanInstance::createSurface()
    {
        VkSurfaceKHR temp_surface;
        VkResult result = glfwCreateWindowSurface(static_cast<VkInstance>(_instance), _glfw_window, nullptr, &temp_surface);
        if (result == VK_FALSE) 
        {
            throw std::runtime_error("failed to create glfw window surface...");
        }

        _surface = vk::SurfaceKHR(temp_surface);
        CHECK_NULL(_surface);
    }

    void VulkanInstance::pickGPU() // maybe improve logic here but that's future thing
    {
        _gpu = _instance.enumeratePhysicalDevices()[0]; // this is the one and only gpu we have
        CHECK_NULL(_gpu);
    }

    void VulkanInstance::createLogicalDevice()
    {
        _utils.findQueueFamilyIndices(_queue_indices);

        float priority = 1.0f;

        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos{};

        if (_queue_indices.isDiscrete()) {
            vk::DeviceQueueCreateInfo queue_create_info[2]{};
            queue_create_info[0].setQueueFamilyIndex(_queue_indices.graphics_family.value())
                                .setQueueCount(1)
                                .setPQueuePriorities(&priority);
            queue_create_info[1].setQueueFamilyIndex(_queue_indices.present_family.value())
                                .setQueueCount(1)
                                .setPQueuePriorities(&priority);

            queue_create_infos.resize(2);
            memcpy(queue_create_infos.data(), queue_create_info, 2*sizeof(vk::DeviceQueueCreateInfo));
        } else {
            vk::DeviceQueueCreateInfo queue_create_info{};

            queue_create_info.setQueueFamilyIndex(_queue_indices.graphics_family.value())
                             .setQueueCount(2)
                             .setPQueuePriorities(&priority);

            queue_create_infos.push_back(queue_create_info);
        }

        const std::vector<const char*> device_extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        vk::DeviceCreateInfo device_create_info{};

        device_create_info.setQueueCreateInfos(queue_create_infos)
                          .setPEnabledExtensionNames(device_extensions)
                          .setEnabledLayerCount(0);

        _logical_device = _gpu.createDevice(device_create_info);
        CHECK_NULL(_logical_device);

        if (_queue_indices.isDiscrete()) {
            _queues.push_back(_logical_device.getQueue(_queue_indices.graphics_family.value(), 0));
            _queues.push_back(_logical_device.getQueue(_queue_indices.present_family.value(), 0));
        } else {
            _queues.push_back(_logical_device.getQueue(_queue_indices.graphics_family.value(), 0));
            _queues.push_back(_logical_device.getQueue(_queue_indices.graphics_family.value(), 1));
        }
        _graphics_queue_index = 0;
        _present_queue_index = 1;
        CHECK_NULL(_queues.at(_graphics_queue_index.value()));
        CHECK_NULL(_queues.at(_present_queue_index.value()));
    }

    void VulkanInstance::createSwapChain()
    {
        SwapchainSupports swapchain_supports{};
        _utils.querySwapChainSupport(swapchain_supports);
        SwapchainDetails swapchain_details{};
        _utils.setSwapchainDetails(swapchain_supports, swapchain_details);

        uint32_t image_count = swapchain_supports.capabilities.minImageCount + 1;
        if (swapchain_supports.capabilities.maxImageCount > 0 
                && image_count > swapchain_supports.capabilities.maxImageCount)
            image_count = swapchain_supports.capabilities.maxImageCount;

        vk::SwapchainCreateInfoKHR swapchain_create_info{};

        swapchain_create_info.setSurface(_surface)
                             .setMinImageCount(image_count)
                             .setImageFormat(swapchain_details.surface_format.format)
                             .setImageColorSpace(swapchain_details.surface_format.colorSpace)
                             .setImageExtent(swapchain_details.extent)
                             .setImageArrayLayers(1)
                             .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

        std::vector<uint32_t> queue_indices = _queue_indices();
        
        if (_queue_indices.isDiscrete()) {
            swapchain_create_info.setImageSharingMode(vk::SharingMode::eConcurrent)
                                 .setQueueFamilyIndices(queue_indices);
        } else {
            swapchain_create_info.setImageSharingMode(vk::SharingMode::eExclusive);
        }

        swapchain_create_info.setPreTransform(swapchain_supports.capabilities.currentTransform)
                             .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
                             .setPresentMode(swapchain_details.present_mode)
                             .setClipped(true);

        _swapchain = _logical_device.createSwapchainKHR(swapchain_create_info);
        CHECK_NULL(_swapchain);
        _swapchain_images = _logical_device.getSwapchainImagesKHR(_swapchain);
        _swapchain_format = swapchain_details.surface_format.format;
        _swapchain_extent = swapchain_details.extent;
    }

    void VulkanInstance::createImageViews()
    {
        _image_views.resize(_swapchain_images.size(), nullptr);

        for (size_t i = 0; i < _image_views.size(); ++i) {
            vk::ImageViewCreateInfo image_view_create_info{};
            image_view_create_info.setImage(_swapchain_images[i])
                                  .setFormat(_swapchain_format)
                                  .setViewType(vk::ImageViewType::e2D)
                                  .setComponents(vk::ComponentMapping())
                                  .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 
                                              0, 1, 0, 1));
            _image_views[i] = _logical_device.createImageView(image_view_create_info);
            CHECK_NULL(_image_views[i]);
        }
    }

    void VulkanInstance::createRenderPass()
    {
        vk::AttachmentDescription color_attachment_description{};
        color_attachment_description.setFormat(_swapchain_format)
                                    .setSamples(vk::SampleCountFlagBits::e1)
                                    .setLoadOp(vk::AttachmentLoadOp::eClear)
                                    .setStoreOp(vk::AttachmentStoreOp::eStore)
                                    .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                                    .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                                    .setInitialLayout(vk::ImageLayout::eUndefined)
                                    .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);
        vk::AttachmentReference color_attachment_reference{};
        color_attachment_reference.setAttachment(0)
                                  .setLayout(vk::ImageLayout::eAttachmentOptimal);
        vk::SubpassDescription subpass_description{};
        subpass_description.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                           .setColorAttachments(color_attachment_reference);
        vk::SubpassDependency subpass_dependency{};
        subpass_dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL)
                          .setDstSubpass(0)
                          .setSrcAccessMask(vk::AccessFlagBits::eNone)
                          .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
                          .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                          .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
        vk::RenderPassCreateInfo render_pass_create_info{};

        render_pass_create_info.setSubpasses(subpass_description)
                               .setAttachments(color_attachment_description)
                               .setDependencies(subpass_dependency);

        _render_pass = _logical_device.createRenderPass(render_pass_create_info);
        CHECK_NULL(_render_pass);
    }

    void VulkanInstance::createCommandPools()
    {
        _command_pools.resize(_swapchain_images.size(), nullptr);
        for (size_t i = 0; i < _command_pools.size(); ++i) {
            vk::CommandPoolCreateInfo command_pool_create_info{};

            command_pool_create_info.setQueueFamilyIndex(_graphics_queue_index.value())
                                    .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
            _command_pools[i] = _logical_device.createCommandPool(command_pool_create_info);
            CHECK_NULL(_command_pools[i]);
        }
        vk::CommandPoolCreateInfo single_time_command_pool_create_info{};
        single_time_command_pool_create_info.setQueueFamilyIndex(_graphics_queue_index.value())
                                            .setFlags(vk::CommandPoolCreateFlagBits::eTransient);
        _single_time_command_pool = _logical_device.createCommandPool(single_time_command_pool_create_info);
        CHECK_NULL(_single_time_command_pool);
    }

    void VulkanInstance::createPrimaryCommandBuffers()
    {
        _primary_command_buffers.resize(_command_pools.size(), nullptr);

        for (size_t i = 0; i < _primary_command_buffers.size(); ++i) {
            vk::CommandBufferAllocateInfo allocate_info{};
            allocate_info
                .setCommandPool(_command_pools[i])
                .setLevel(vk::CommandBufferLevel::ePrimary)
                .setCommandBufferCount(1);

            _primary_command_buffers[i] = _logical_device.allocateCommandBuffers(allocate_info)[0];
            CHECK_NULL(_primary_command_buffers[i]);
        }

        _secondary_command_buffers.resize(_primary_command_buffers.size());
    }

    void VulkanInstance::createSecondaryCommandBuffer(uint32_t image_index, uint32_t secondary_buffer_index)
    {
        uint32_t allocate_count = secondary_buffer_index - _secondary_command_buffers.at(image_index).size() + 1;
        if (allocate_count <= 0) return;
        vk::CommandBufferAllocateInfo allocate_info{};
        allocate_info
            .setCommandPool(_command_pools[image_index])
            .setLevel(vk::CommandBufferLevel::eSecondary)
            .setCommandBufferCount(allocate_count);

        std::vector<vk::CommandBuffer> temp_buffers = _logical_device.allocateCommandBuffers(allocate_info);

        for (size_t i = 0; i < temp_buffers.size(); ++i) {
            CHECK_NULL(temp_buffers.at(i));
        }

        _secondary_command_buffers.at(image_index)
            .insert(_secondary_command_buffers.at(image_index).end(), 
                    temp_buffers.begin(), 
                    temp_buffers.end());
    }

    void VulkanInstance::createFramebuffers()
    {
        _framebuffers.resize(_swapchain_images.size());

        for (size_t i = 0; i < _framebuffers.size(); ++i) {
            vk::FramebufferCreateInfo framebuffer_create_info{};
            framebuffer_create_info.setRenderPass(_render_pass)
                                   .setAttachments(_image_views[i])
                                   .setWidth(_swapchain_extent.width)
                                   .setHeight(_swapchain_extent.height)
                                   .setLayers(1);
            _framebuffers[i] = _logical_device.createFramebuffer(framebuffer_create_info);
            CHECK_NULL(_framebuffers[i]);
        }
    }

    GLFWwindow* VulkanInstance::getGLFWwindow() const { return _glfw_window; }
    vk::PhysicalDevice VulkanInstance::getGPU() const { return _gpu; }
    vk::Device VulkanInstance::getLogicalDevice() const { return _logical_device; }
    vk::SurfaceKHR VulkanInstance::getSurface() const { return _surface; }

    void VulkanInstance::cleanInstance() { _instance.destroy(); }
    void VulkanInstance::cleanSurface() { _instance.destroySurfaceKHR(_surface); }
    void VulkanInstance::cleanLogicalDevice() { _logical_device.destroy(); }
    void VulkanInstance::cleanSwapchain()
    {
        for (auto image_view : _image_views) {
            _logical_device.destroy(image_view);
        }

        for (auto swapchain_image : _swapchain_images) {
            _logical_device.destroy(swapchain_image);
        }
        
        _logical_device.destroy(_swapchain);

    }

}

#include "Vulkan/UnMacro.hh"
