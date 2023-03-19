#include "Core/Renderer.hh"

#include "Core/ResourceManager.hh"
#include "Vulkan/VulkanInstance.hh"
#include "Vulkan/VulkanRenderEntity.hh"

#include <GLFW/glfw3.h>
#include <toml++/toml.h>

namespace DWE {
    Renderer::Renderer(GLFWwindow* window) 
        : _vulkan_instance(std::make_unique<VulkanInstance>(window))
    {}

    VulkanInstance* Renderer::getVulkanInstance() const
    {
        return _vulkan_instance.get();
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
        uint32_t image_index = _vulkan_instance->waitAvailableFrameBuffer();
        _vulkan_instance->getRenderCommandBufferBegin(image_index);
        {
            for (auto entity : _render_entities) {
                entity->writeDrawingCommands(image_index);
            }
        }
        _vulkan_instance->getRenderCommandBufferend(image_index);
        _vulkan_instance->submitCommands(image_index);
        _vulkan_instance->presentFrame();
    }
}
