#include "Core/Application.hh"

#include <assimp/Importer.hpp>

#include <toml++/toml.h>

namespace DWE {
    Application::Application()
    {
        std::cout << "hello project\n";
        _window = Window::getInstance();
        _on_event = std::bind(&Application::onEvent, this, std::placeholders::_1);
        _window->setEventCallback(_on_event);
#ifdef DWE_DEBUG
        _logger = new Logger("AppConsole");
        _logger->setPattern("%^[%T] %n: %v%$");
        _logger->setLevel(LogLevel::trace);
        _logger->flushOn(LogLevel::trace);
#endif
        _renderer = new Renderer{_window->getGLFWwindowPointer()};
    }

    Application::~Application()
    {
        std::cout << "goodbye project\n";
    }

    void Application::run()
    {
        _running = true;
        std::cout << "project says hello too\n";
        while(_running) {
            _window->update();
        }
    }

    void Application::onEvent(Event& e)
    {
        auto event_type = e.getEventType();
        switch(event_type) {
            case EventType::WindowClose:
            {
                _running = false;
            } break;
            default: break;
        }

#ifdef DWE_DEBUGG
        _logger->log(LogLevel::trace, e.verbose().c_str());
#endif
    }
}
