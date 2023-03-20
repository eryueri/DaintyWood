#include "Core/Application.hh"

#include "Vulkan/EntitySettings.hh"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

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
        _resource_manager = new ResourceManager{_renderer->getVulkanInstance()};
        _renderer->setResourceManager(_resource_manager);

        _resource_manager->loadFile("config.toml");
        _renderer->prepareEntities();
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
            _renderer->renderFrame();
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
            case EventType::KeyPress:
            {
                _running = false; // temporary for fast debug
            }
            default: break;
        }

#ifdef DWE_DEBUGG
        _logger->log(LogLevel::trace, e.verbose().c_str());
#endif
    }
}
