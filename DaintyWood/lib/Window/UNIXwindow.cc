#include "Window/UNIXwindow.hh"

#include <GLFW/glfw3.h>

#include "Events/KeyEvents.hh"
#include "Events/MouseEvents.hh"
#include "Events/WindowEvents.hh"

namespace DWE {
    UNIXwindow* UNIXwindow::_instance = nullptr;
    UNIXwindow::UNIXwindow(WindowProperties props)
    {
        if (_instance == nullptr) 
        {
            glfwInit();
        }

        _glfw_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
    }

    UNIXwindow* UNIXwindow::getInstance()
    {
        if (_instance == nullptr)
        {
            _instance = new UNIXwindow{WindowProperties{}};
        }
        return _instance;
    }

    void UNIXwindow::update()
    {
        while(!glfwWindowShouldClose(_glfw_window))
        {
            glfwPollEvents();
        }
        glfwTerminate();
    }

    void UNIXwindow::setEventCallback(const CallbackFunc& func)
    {
        _callback = func;

        glfwSetWindowUserPointer(_glfw_window, &_callback);

        glfwSetWindowCloseCallback(
            _glfw_window, 
            [](GLFWwindow* window)
            {
                WindowCloseEvent e{};
                CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
                (*func)(e);
            }
        );

        glfwSetWindowSizeCallback(
            _glfw_window, 
            [](GLFWwindow* window, int width, int height)
            {
                WindowResizeEvent e{width, height};
                CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
                (*func)(e);
            }
        );

        glfwSetWindowPosCallback(
            _glfw_window, 
            [](GLFWwindow* window, int xpos, int ypos)
            {
                WindowMoveEvent e{xpos, ypos};
                CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
                (*func)(e);
            }
        );

        glfwSetWindowFocusCallback(
            _glfw_window, 
            [](GLFWwindow* window, int focused)
            {
                WindowFocusEvent e{focused};
                CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
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
                    CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
                    (*func)(e);
                } break;
                case GLFW_RELEASE: {
                    KeyReleaseEvent e{key};
                    CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
                    (*func)(e);
                } break;
                case GLFW_REPEAT: {
                    KeyRepeatEvent e{key};
                    CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
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
                    CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
                    (*func)(e);
                } break;
                case GLFW_RELEASE: {
                    MouseButtonReleaseEvent e{button};
                    CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
                    (*func)(e);
                } break;
                case GLFW_REPEAT: {
                    MouseButtonRepeatEvent e{button};
                    CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
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
                CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
                (*func)(e);
            }
        );

        glfwSetScrollCallback(
            _glfw_window, 
            [](GLFWwindow* window, double xoffset, double yoffset)
            {
                MouseScrollEvent e{(float)xoffset, (float)yoffset};
                CallbackFunc* func = (CallbackFunc*)glfwGetWindowUserPointer(window);
                (*func)(e);
            }
        );
    }
}
