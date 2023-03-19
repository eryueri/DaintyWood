#pragma once

#include "Core/Logger.hh"
#include "Core/Window.hh"
#include "Core/Renderer.hh"
#include "Core/ResourceManager.hh"

#include "pch.hh"

namespace DWE {
    class Application {
    public:
        Application();
        ~Application();
        void run();

        void onEvent(Event& e);
    private:
        bool _running = false;
        Window* _window = nullptr;
        EventCallbackFunc _on_event;

        Renderer* _renderer = nullptr;
        ResourceManager* _resource_manager = nullptr;

#ifdef DWE_DEBUG
    private:
        Logger* _logger;
#endif
    };
}
