#include "Core/Application.hh"

#include "Window/UNIXwindow.hh"

#include "Events/KeyEvents.hh"

namespace DWE {
    Application::Application()
    {
        std::cout << "hello project\n";
        _window = UNIXwindow::getInstance();
        _on_event = std::bind(&Application::onEvent, this, std::placeholders::_1);
        _window->setEventCallback(_on_event);
    }

    Application::~Application()
    {
        std::cout << "goodbye project\n";
    }

    void Application::run()
    {
        std::cout << "project says hello too\n";
        _window->update();
    }

    void Application::onEvent(Event& e)
    {
#ifdef DWE_DEBUG
        std::cerr << e.verbose() << std::endl;
#endif
    }
}
