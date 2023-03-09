#include "Core/Window.hh"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Events/KeyEvents.hh"
#include "Events/MouseEvents.hh"
#include "Events/WindowEvents.hh"

namespace DWE {
    Window* Window::_instance = nullptr;
    Window::Window(WindowProperties props)
    {
        if (_instance == nullptr) 
        {
            glfwInit();
        }

        _glfw_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
    }

    Window* Window::getInstance()
    {
        if (_instance == nullptr)
        {
            _instance = new Window{WindowProperties{}};
        }
        return _instance;
    }

    void Window::update()
    {
        while(!glfwWindowShouldClose(_glfw_window))
        {
            glfwPollEvents();
        }
        glfwTerminate();
    }

    void Window::setEventCallback(const EventCallbackFunc& func)
    {
        _event_callback = func;

        glfwSetWindowUserPointer(_glfw_window, &_event_callback);

        glfwSetWindowCloseCallback(
            _glfw_window, 
            [](GLFWwindow* window)
            {
                WindowCloseEvent e{};
                EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                (*func)(e);
            }
        );

        glfwSetWindowSizeCallback(
            _glfw_window, 
            [](GLFWwindow* window, int width, int height)
            {
                WindowResizeEvent e{width, height};
                EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                (*func)(e);
            }
        );

        glfwSetWindowPosCallback(
            _glfw_window, 
            [](GLFWwindow* window, int xpos, int ypos)
            {
                WindowMoveEvent e{xpos, ypos};
                EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                (*func)(e);
            }
        );

        glfwSetWindowFocusCallback(
            _glfw_window, 
            [](GLFWwindow* window, int focused)
            {
                WindowFocusEvent e{focused};
                EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                (*func)(e);
            }
        );

        glfwSetKeyCallback(
            _glfw_window, 
            [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                switch(action) {
                case GLFW_PRESS: {
                    KeyPressEvent e{key};
                    EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                    (*func)(e);
                } break;
                case GLFW_RELEASE: {
                    KeyReleaseEvent e{key};
                    EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                    (*func)(e);
                } break;
                case GLFW_REPEAT: {
                    KeyRepeatEvent e{key};
                    EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                    (*func)(e);
                } break;
                default: {
                } break;
                }
            }
        );

        glfwSetMouseButtonCallback(
            _glfw_window, 
            [](GLFWwindow* window, int button, int action , int mods)
            {
                switch(action) {
                case GLFW_PRESS: {
                    MouseButtonPressEvent e{button};
                    EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                    (*func)(e);
                } break;
                case GLFW_RELEASE: {
                    MouseButtonReleaseEvent e{button};
                    EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                    (*func)(e);
                } break;
                case GLFW_REPEAT: {
                    MouseButtonRepeatEvent e{button};
                    EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                    (*func)(e);
                } break;
                default: {
                } break;
                }
            }
        );

        glfwSetCursorPosCallback(
            _glfw_window, 
            [](GLFWwindow* window, double xpos, double ypos)
            {
                MouseMoveEvent e{(float)xpos, (float)ypos};
                EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                (*func)(e);
            }
        );

        glfwSetScrollCallback(
            _glfw_window, 
            [](GLFWwindow* window, double xoffset, double yoffset)
            {
                MouseScrollEvent e{(float)xoffset, (float)yoffset};
                EventCallbackFunc* func = (EventCallbackFunc*)glfwGetWindowUserPointer(window);
                (*func)(e);
            }
        );
    }

    GLFWwindow* Window::getGLFWwindowPointer()
    {
        return _glfw_window;
    }
}
