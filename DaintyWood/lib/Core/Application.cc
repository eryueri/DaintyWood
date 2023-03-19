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
    }

    Application::~Application()
    {
        std::cout << "goodbye project\n";
    }

    void Application::loadEntity(std::string filename)
    {
        auto file = toml::parse_file(filename);

        std::string mesh_config = file["MeshConfig"]["file"].value_or("");
        std::string texure_config = file["TextureConfig"]["file"].value_or("");
        std::string shader_config = file["ShaderConfig"]["file"].value_or("");

        EntitySettings entity_setting{};
        if (!mesh_config.empty()) entity_setting.mesh_configs.push_back(mesh_config);
        if (!texure_config.empty()) entity_setting.texture_configs.push_back(texure_config);
        if (!shader_config.empty()) entity_setting.shader_configs.push_back(shader_config);

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
