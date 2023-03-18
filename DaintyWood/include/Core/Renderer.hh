#pragma once

#include "Vulkan/VulkanEntity.hh"

struct GLFWwindow;
namespace DWE {
    class VulkanInstance;
    class VulkanRenderEntity;
    class ResourceManager;
    class Renderer {
    public:
        Renderer() = delete;
        Renderer(GLFWwindow* window);
        VulkanInstance* getVulkanInstance() const;
        void setResourceManager(ResourceManager* resource_manager);
        void loadEntity(); // this is temporary, entities will be managed in another way
        void renderFrame();
    private:
        std::unique_ptr<VulkanInstance> _vulkan_instance;
        ResourceManager* _resource_manager = nullptr;
        std::vector<VulkanEntity*> _render_entities;
    };
}
