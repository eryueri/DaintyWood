#include "Events/KeyEvents.hh"

namespace DWE {
#ifdef DWE_DEBUG
    std::string KeyPressEvent::verbose()
    {
        std::string str("Key Pressed: ");
        str += std::to_string(_code);
        return str;
    }

    std::string KeyReleaseEvent::verbose()
    {
        std::string str("Key Released: ");
        str += std::to_string(_code);
        return str;
    }

    std::string KeyRepeatEvent::verbose()
    {
        std::string str("Key Repeated: ");
        str += std::to_string(_code);
        return str;
    }
#endif

}
