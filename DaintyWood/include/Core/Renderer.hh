#pragma once

#include "Vulkan/VulkanEntity.hh"

struct GLFWwindow;
namespace DWE {
    class VulkanInstance;
    class VulkanRenderEntity;
    class Renderer {
    public:
        Renderer() = delete;
        Renderer(GLFWwindow* window);
        void loadEntity(VulkanEntity entity); // this is temporary, entities will be managed in another way
        void renderFrame();
    private:
        std::unique_ptr<VulkanInstance> _vulkan_instance;
        std::vector<VulkanRenderEntity> _render_entities;
    };
}
