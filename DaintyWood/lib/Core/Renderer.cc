#include "Core/Renderer.hh"

#include "Vulkan/VulkanInstance.hh"
#include "Vulkan/VulkanRenderEntity.hh"

#include <GLFW/glfw3.h>
#include <toml++/toml.h>

namespace DWE {
    Renderer::Renderer(GLFWwindow* window) 
        : _vulkan_instance(std::make_unique<VulkanInstance>(window))
    {}

    void Renderer::loadEntity(VulkanEntity entity)
    {
        
    }

    void Renderer::renderFrame()
    {
        uint32_t image_index = _vulkan_instance->waitAvailableFrameBuffer();
        _vulkan_instance->getRenderCommandBufferBegin(image_index);
        {
            for (auto entity : _render_entities) {
                entity.writeDrawingCommands();
            }
        }
        _vulkan_instance->getRenderCommandBufferend(image_index);
        _vulkan_instance->submitCommands(image_index);
        _vulkan_instance->presentFrame();
    }
}
