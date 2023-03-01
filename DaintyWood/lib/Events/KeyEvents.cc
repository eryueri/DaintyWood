#include "Events/KeyEvents.hh"

namespace DWE {
#ifdef DWE_DEBUG
    const char* KeyPressEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*20);
        sprintf(_debug_buffer, "Key Pressed: %d", _code);
        return _debug_buffer;
    }

    const char* KeyReleaseEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*20);
        sprintf(_debug_buffer, "Key Released: %d", _code);
        return _debug_buffer;
    }

    const char* KeyRepeatEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*20);
        sprintf(_debug_buffer, "Key Repeated: %d", _code);
        return _debug_buffer;
    }
#endif
    KeyPressEvent::~KeyPressEvent()
    {
#ifdef DWE_DEBUG
        if(_debug_buffer)
            free(_debug_buffer);
#endif
    }

    KeyReleaseEvent::~KeyReleaseEvent()
    {
#ifdef DWE_DEBUG
        if(_debug_buffer)
            free(_debug_buffer);
#endif
    }

    KeyRepeatEvent::~KeyRepeatEvent()
    {
#ifdef DWE_DEBUG
        if(_debug_buffer)
            free(_debug_buffer);
#endif
    }

}
