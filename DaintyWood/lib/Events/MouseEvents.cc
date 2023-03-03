#include "Events/MouseEvents.hh"

namespace DWE {
#ifdef DWE_DEBUG

    std::string MouseMoveEvent::verbose()
    {
        std::string str("Mouse moved to: ");
        str = str + std::to_string(_x_pos) + ", " + std::to_string(_y_pos);
        return str;
    }

    std::string MouseButtonPressEvent::verbose()
    {
        std::string str("Mouse button pressed: ");
        str += std::to_string(_code);
        return str;
    }

    std::string MouseButtonReleaseEvent::verbose()
    {
        std::string str("Mouse button released: ");
        str += std::to_string(_code);
        return str;
    }

    std::string MouseButtonRepeatEvent::verbose()
    {
        std::string str("Mouse button repeated: ");
        str += std::to_string(_code);
        return str;
    }

    std::string MouseScrollEvent::verbose()
    {
        std::string str("Mouse button pressed: ");
        str = str +std::to_string(_x_offset) + ", " + std::to_string(_y_offset);
        return str;
    }

#endif

}
