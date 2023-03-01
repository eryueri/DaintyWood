#pragma once

#include "pch.hh"
#include "Events/Event.hh"

namespace DWE {
    struct WindowProperties {
        uint32_t width, height;
        std::string title;
        WindowProperties() 
            : width(1280), height(960), 
              title("Demo") {}
    };
    using CallbackFunc=std::function<void(Event&)>;
    class Window {
    public:
        virtual void update() = 0;

        virtual void setEventCallback(const CallbackFunc& func) = 0;
    };
}
