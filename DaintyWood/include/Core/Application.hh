#pragma once

#include "Core/Logger.hh"
#include "Core/Window.hh"
#include "Vulkan/VulkanInstance.hh"

#include "pch.hh"

namespace DWE {
    class Application {
    public:
        Application();
        ~Application();
        void run();

        void onEvent(Event& e);
    private:
        Window* _window = nullptr;
        VulkanInstance* _vulkan_instance = nullptr;
        EventCallbackFunc _on_event;

#ifdef DWE_DEBUG
    private:
        Logger* _logger;
#endif
    };
}
