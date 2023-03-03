#include "Events/WindowEvents.hh"

namespace DWE {
#ifdef DWE_DEBUG
    std::string WindowCloseEvent::verbose()
    {
        std::string str("Window is closed!");
        return str;
    }

    std::string WindowResizeEvent::verbose()
    {
        std::string str("Window is resized to: ");
        str = str + std::to_string(_resize_width) + ", " + std::to_string(_resize_height);
        return str;
    }

    std::string WindowMoveEvent::verbose()
    {
        std::string str("Window is moved to: ");
        str = str + std::to_string(_move_to_x) + ", " + std::to_string(_move_to_y);
        return str;
    }

    std::string WindowFocusEvent::verbose()
    {
        std::string str;
        if (_focused)
            str += "Window is focused!";
        else
            str += "Window is unfocused!";
        return str;
    }

#endif

}
