#pragma once

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
    };
}
