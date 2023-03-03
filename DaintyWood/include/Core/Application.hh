#pragma once

#include "Core/Logger.hh"
#include "Window/Window.hh"

#include "pch.hh"

namespace DWE {
    class Application {
    public:
        Application();
        ~Application();
        void run();

        void onEvent(Event& e);
    private:
        Window* _window;
        CallbackFunc _on_event;

#ifdef DWE_DEBUG
    private:
        Logger* _logger;
#endif
    };
}
