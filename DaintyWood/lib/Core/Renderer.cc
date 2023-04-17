#include "Core/Renderer.hh"

#include "Core/ResourceManager.hh"
#include "Vulkan/VulkanInstance.hh"
#include "Vulkan/VulkanRenderEntity.hh"

#include <GLFW/glfw3.h>
#include <toml++/toml.h>

namespace DWE {
    Renderer::Renderer(GLFWwindow* window) 
        : _vulkan_instance(std::make_unique<VulkanInstance>(window))
    {
        CameraProperties camera_info;
        camera_info.position={2.0f, 2.0f, 2.0f};
        camera_info.look_at = {0.0f, 0.0f, 0.0f};
        camera_info.up_direction = {0.0f, 0.0f, -1.0f};
        camera_info.fov = 45.0f;
        camera_info.aspect = getAspectRatio();
        camera_info.z_far = 10.0f;
        camera_info.z_near = 0.1f;

        _camera = new Camera{camera_info};
    }

    VulkanInstance* Renderer::getVulkanInstance() const
    {
        return _vulkan_instance.get();
    }

    float Renderer::getAspectRatio() 
    {
        auto extent = _vulkan_instance->getSwapchainExtent();

        return extent.width / (float)extent.height;
    }

    void Renderer::setResourceManager(ResourceManager *resource_manager)
    {
        _resource_manager = resource_manager;
    }

    void Renderer::prepareEntities()
    {
        std::vector<VulkanEntity*> entities = _resource_manager->getRenderEntities();
        _render_entities.insert(_render_entities.end(), entities.begin(), entities.end());
    }

    void Renderer::renderFrame()
    {
        UniformData data{};
        _camera->writeUniformData(data);
        uint32_t image_index = _vulkan_instance->waitAvailableFrameBuffer();
        _vulkan_instance->getRenderCommandBufferBegin(image_index);
        {
            for (auto entity : _render_entities) {
                entity->updateUniformData(image_index, data);
                entity->writeDrawingCommands(image_index);
            }
        }
        _vulkan_instance->getRenderCommandBufferend(image_index);
        _vulkan_instance->submitCommands(image_index);
        _vulkan_instance->presentFrame();
    }
}
