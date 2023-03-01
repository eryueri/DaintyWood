#pragma once

#include "Window.hh"

struct GLFWwindow;

namespace DWE {
    class UNIXwindow : public Window{
    public:
        ~UNIXwindow();

        static UNIXwindow* getInstance();

        void update() override;

        void setEventCallback(const CallbackFunc& func) override;

    private:
        UNIXwindow() = delete;
        UNIXwindow(WindowProperties props);
        GLFWwindow* _glfw_window = nullptr;
        CallbackFunc _callback;
        static UNIXwindow* _instance;
    };
}
