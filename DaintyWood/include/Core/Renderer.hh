#pragma once

#include "Vulkan/VulkanEntity.hh"
#include "Vulkan/UniformData.hh"

#include "Core/Camera.hh"

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
        void prepareEntities();
        void renderFrame();
        float getAspectRatio();

        void updateEntity(VulkanEntity* entity, const UniformData& data);
    private:
        Camera* _camera;
    private:
        std::unique_ptr<VulkanInstance> _vulkan_instance;
        ResourceManager* _resource_manager = nullptr;
        std::vector<VulkanEntity*> _render_entities;
    };
}
