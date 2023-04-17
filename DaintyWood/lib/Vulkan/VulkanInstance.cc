#include "Vulkan/VulkanInstance.hh"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Vulkan/VulkanUtils.hh"

#include "Vulkan/Macro.hh"
namespace DWE {
    VulkanInstance::VulkanInstance(GLFWwindow* window) 
        : _glfw_window(window), 
        _util(std::make_unique<VulkanUtils>(this))
    {
        _clear_values[0].color = vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f};
        _clear_values[1].depthStencil = vk::ClearDepthStencilValue{1.0f, 0};

        createInstance();
        createSurface();
        pickGPU();
        createLogicalDevice();
        createSwapChain();
        createImageViews();
        createDepthResources();
        createRenderPass();
        createCommandPools();
        createPrimaryCommandBuffers();
        createFramebuffers();
        createSyncObjects();
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
        if (result != VK_SUCCESS) 
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
        vk::QueueFamilyProperties queue_properties = _util->findQueueFamilyIndices(_queue_indices);

        float priority = 1.0f;

        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos{};

        uint32_t queue_count = 1;

        if (_queue_indices.isDiscrete()) {
            vk::DeviceQueueCreateInfo queue_create_info[2]{};
            queue_create_info[0].setQueueFamilyIndex(_queue_indices.graphics_family.value())
                                .setQueueCount(1)
                                .setPQueuePriorities(&priority);
            queue_create_info[1].setQueueFamilyIndex(_queue_indices.present_family.value())
                                .setQueueCount(1)
                                .setPQueuePriorities(&priority);

            queue_create_infos.push_back(queue_create_info[0]);
            queue_create_infos.push_back(queue_create_info[1]);
        } else {
            vk::DeviceQueueCreateInfo queue_create_info{};
            if (queue_properties.queueCount >= 2) {
                queue_count = 2;
            }

            queue_create_info.setQueueFamilyIndex(_queue_indices.graphics_family.value())
                             .setQueueCount(queue_count)
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
            if (queue_count == 2)
                _queues.push_back(_logical_device.getQueue(_queue_indices.graphics_family.value(), 1));
            else 
                _queues.push_back(_logical_device.getQueue(_queue_indices.graphics_family.value(), 0));
        }
        _graphics_queue_index = 0;
        _present_queue_index = 1;
        CHECK_NULL(_queues.at(_graphics_queue_index.value()));
        CHECK_NULL(_queues.at(_present_queue_index.value()));
    }

    void VulkanInstance::createSwapChain()
    {
        SwapchainSupports swapchain_supports{};
        _util->querySwapChainSupport(swapchain_supports);
        SwapchainDetails swapchain_details{};
        _util->setSwapchainDetails(swapchain_supports, swapchain_details);

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
                                  .setLayout(vk::ImageLayout::eColorAttachmentOptimal);
        vk::SubpassDescription subpass_description{};
        subpass_description.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                           .setColorAttachments(color_attachment_reference);

        vk::AttachmentDescription depth_attachment_description{};
        depth_attachment_description
            .setFormat(_util->findSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint}, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment))
            .setSamples(vk::SampleCountFlagBits::e1)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

        vk::AttachmentReference depth_attachment_reference{};
        depth_attachment_reference
            .setAttachment(1)
            .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

        vk::SubpassDescription depth_subpass_description{};
        depth_subpass_description
            .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
            .setColorAttachments(depth_attachment_reference);

        std::vector<vk::AttachmentDescription> attachments = {color_attachment_description, depth_attachment_description};

        vk::SubpassDependency subpass_dependency{};
        subpass_dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL)
                          .setDstSubpass(0)
                          .setSrcAccessMask(vk::AccessFlagBits::eNone)
                          .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
                          .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
                          .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests);
        vk::RenderPassCreateInfo render_pass_create_info{};

        render_pass_create_info.setSubpasses(subpass_description)
                               .setAttachments(attachments)
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
            std::vector<vk::ImageView> attachments = {_image_views[i], _depth_image_view};
            vk::FramebufferCreateInfo framebuffer_create_info{};
            framebuffer_create_info.setRenderPass(_render_pass)
                                   .setAttachments(attachments)
                                   .setWidth(_swapchain_extent.width)
                                   .setHeight(_swapchain_extent.height)
                                   .setLayers(1);
            _framebuffers[i] = _logical_device.createFramebuffer(framebuffer_create_info);
            CHECK_NULL(_framebuffers[i]);
        }
    }

    void VulkanInstance::createSyncObjects()
    {
        _image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
        _render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
        _inflight_fences.resize(MAX_FRAMES_IN_FLIGHT);

        vk::SemaphoreCreateInfo semaphore_create_info{};

        vk::FenceCreateInfo fence_create_info{};
        fence_create_info.setFlags(vk::FenceCreateFlagBits::eSignaled);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            _image_available_semaphores[i] = _logical_device.createSemaphore(semaphore_create_info);
            CHECK_NULL(_image_available_semaphores[i]);
            _render_finished_semaphores[i] = _logical_device.createSemaphore(semaphore_create_info);
            CHECK_NULL(_render_finished_semaphores[i]);
            _inflight_fences[i] = _logical_device.createFence(fence_create_info);
            CHECK_NULL(_inflight_fences[i]);
        }
    }

    void VulkanInstance::createDepthResources()
    {
        vk::Format depth_format = _util->findSupportedFormat( // find depth format
                {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint}, 
                vk::ImageTiling::eOptimal, 
                vk::FormatFeatureFlagBits::eDepthStencilAttachment);

        _util->allocateImage(_swapchain_extent.width, 
                _swapchain_extent.height, 
                depth_format, 
                vk::ImageTiling::eOptimal, 
                vk::ImageUsageFlagBits::eDepthStencilAttachment, 
                vk::MemoryPropertyFlagBits::eDeviceLocal, 
                _depth_image, 
                _depth_memory);

        _util->createImageView(
                _depth_image, 
                depth_format, 
                _depth_image_view, 
                vk::ImageAspectFlagBits::eDepth);
    }

    vk::CommandBuffer VulkanInstance::getSingleTimeCommandsBegin()
    {
        vk::CommandBufferAllocateInfo allocate_info{};
        allocate_info
            .setCommandPool(_single_time_command_pool)
            .setLevel(vk::CommandBufferLevel::ePrimary)
            .setCommandBufferCount(1);
        _single_time_command_buffer = _logical_device.allocateCommandBuffers(allocate_info)[0];
        CHECK_NULL(_single_time_command_buffer);

        vk::CommandBufferBeginInfo begin_info{};
        begin_info
            .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
            .setPInheritanceInfo(nullptr);

        _single_time_command_buffer.begin(begin_info);

        return _single_time_command_buffer;
    }

    void VulkanInstance::getSingleTimeCommandsEnd()
    {
        _single_time_command_buffer.end();

        vk::SubmitInfo submit_info{};
        submit_info
            .setCommandBuffers(_single_time_command_buffer);
        _queues[_graphics_queue_index.value()].submit(submit_info, nullptr);
        _queues[_graphics_queue_index.value()].waitIdle();

        _logical_device.freeCommandBuffers(_single_time_command_pool, _single_time_command_buffer);
        _single_time_command_buffer = nullptr;
    }

    uint32_t VulkanInstance::waitAvailableFrameBuffer()
    {
        vk::Result result = _logical_device.acquireNextImageKHR(_swapchain, std::numeric_limits<uint64_t>::max(), _image_available_semaphores[_current_frame], nullptr, &_current_image);
        result = _logical_device.waitForFences(1, &_inflight_fences[_current_frame], true, std::numeric_limits<uint64_t>::max());

        result = _logical_device.resetFences(1, &_inflight_fences[_current_frame]);

        return _current_image;
    }

    vk::CommandBuffer VulkanInstance::getRenderCommandBuffer(uint32_t image_index) const
    {
        return _primary_command_buffers[image_index];
    }

    void VulkanInstance::getRenderCommandBufferBegin(uint32_t image_index)
    {
        _logical_device.resetCommandPool(_command_pools[image_index]);

        vk::CommandBuffer buffer = _primary_command_buffers[image_index];

        vk::CommandBufferBeginInfo begin_info{};
        begin_info
            .setFlags(vk::CommandBufferUsageFlags(0))
            .setPInheritanceInfo(nullptr);
        buffer.begin(begin_info);

        vk::Viewport viewport{
            0.0f, 0.0f, // x, y
            (float)_swapchain_extent.width, (float)_swapchain_extent.height, 
            0.0f, 1.0f  // min/max depth
        };

        vk::Rect2D scissor{
            vk::Offset2D{ 0, 0 }, 
            _swapchain_extent
        };

        vk::RenderPassBeginInfo render_pass_begin_info{};
        render_pass_begin_info
            .setRenderPass(_render_pass)
            .setFramebuffer(_framebuffers[image_index])
            .setRenderArea(scissor)
            .setClearValues(_clear_values);

        buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
        buffer.setViewport(0, 1, &viewport);
        buffer.setScissor(0, 1, &scissor);
    }

    void VulkanInstance::getRenderCommandBufferend(uint32_t image_index)
    {
        _primary_command_buffers[image_index].endRenderPass();
        _primary_command_buffers[image_index].end();
    }
    
    void VulkanInstance::submitCommands(uint32_t image_index)
    {
        vk::PipelineStageFlags wait_stages{vk::PipelineStageFlagBits::eColorAttachmentOutput};

        vk::SubmitInfo submit_info{};
        submit_info
            .setWaitSemaphoreCount(1)
            .setPWaitSemaphores(&_image_available_semaphores[_current_frame])
            .setSignalSemaphoreCount(1)
            .setPSignalSemaphores(&_render_finished_semaphores[_current_frame])
            .setWaitDstStageMask(wait_stages)
            .setCommandBufferCount(1)
            .setPCommandBuffers(&_primary_command_buffers[image_index]);

        vk::Result result = _queues[_graphics_queue_index.value()].submit(1, &submit_info, _inflight_fences[_current_frame]);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to submit commands...");
        }
    }

    void VulkanInstance::presentFrame()
    {
        vk::PresentInfoKHR present_info{};
        present_info
            .setWaitSemaphoreCount(1)
            .setPWaitSemaphores(&_render_finished_semaphores[_current_frame])
            .setSwapchainCount(1)
            .setPSwapchains(&_swapchain)
            .setImageIndices(_current_image);

        vk::Result result = _queues[_present_queue_index.value()].presentKHR(present_info);

        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("failed to present image to swapchain...");
        }

        _current_frame = (_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    GLFWwindow* VulkanInstance::getGLFWwindow() const { return _glfw_window; }
    VulkanUtils* VulkanInstance::getUtil() const { return _util.get(); }
    vk::PhysicalDevice VulkanInstance::getGPU() const { return _gpu; }
    vk::Device VulkanInstance::getLogicalDevice() const { return _logical_device; }
    vk::SurfaceKHR VulkanInstance::getSurface() const { return _surface; }
    vk::RenderPass VulkanInstance::getRenderPass() const { return _render_pass; }
    vk::Extent2D VulkanInstance::getSwapchainExtent() const { return _swapchain_extent; }
    uint32_t VulkanInstance::getSwapchainSize() const { return _swapchain_images.size(); }
    uint32_t VulkanInstance::getMemoryType(uint32_t filter, vk::MemoryPropertyFlags prop) const
    {
        vk::PhysicalDeviceMemoryProperties properties = _gpu.getMemoryProperties();
        for (uint32_t i = 0; i < properties.memoryTypeCount; ++i) {
            if ((filter & (i << i)) && ((properties.memoryTypes[i].propertyFlags & prop) == prop)) {
                return i;
            }
        }
        throw std::runtime_error("cannot get proper memory type...");
    }
    VulkanUtils* VulkanInstance::getVulkanUtils() const { return _util.get(); }

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
