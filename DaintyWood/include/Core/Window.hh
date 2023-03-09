#pragma once

#include "pch.hh"
#include "Events/Event.hh"

struct GLFWwindow;

namespace vk {
    class Instance;
}

namespace DWE {
    struct WindowProperties {
        uint32_t width, height;
        std::string title;
        WindowProperties() 
            : width(1280), height(960), 
              title("Demo") {}
    };
    using EventCallbackFunc = std::function<void(Event&)>;
    class Window {
    public:
        static Window* getInstance();

        void update();

        void setEventCallback(const EventCallbackFunc& func);

        void createSurface(vk::Instance* instance);

        GLFWwindow* getGLFWwindowPointer();

    private:
        Window() = delete;
        Window(WindowProperties props);
        GLFWwindow* _glfw_window = nullptr;
        EventCallbackFunc _event_callback;
        static Window* _instance;
    };
}
