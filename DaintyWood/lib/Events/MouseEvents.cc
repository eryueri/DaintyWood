#include "Events/MouseEvents.hh"

namespace DWE {
#ifdef DWE_DEBUG

    const char* MouseMoveEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*36);
        sprintf(_debug_buffer, "Mouse moves to: %f, %f", _x_pos, _y_pos);
        return _debug_buffer;
    }

    const char* MouseButtonPressEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*32);
        sprintf(_debug_buffer, "Mouse button pressed: %d", _code);
        return _debug_buffer;
    }

    const char* MouseButtonReleaseEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*32);
        sprintf(_debug_buffer, "Mouse button released: %d", _code);
        return _debug_buffer;
    }

    const char* MouseButtonRepeatEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*32);
        sprintf(_debug_buffer, "Mouse button repeated: %d", _code);
        return _debug_buffer;
    }

    const char* MouseScrollEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*36);
        sprintf(_debug_buffer, "Mouse scrolled: %f, %f", _x_offset, _y_offset);
        return _debug_buffer;
    }

#endif

    MouseMoveEvent::~MouseMoveEvent()
    {
#ifdef DWE_DEBUG
        if (_debug_buffer)
            free(_debug_buffer);
#endif
    }

    MouseButtonPressEvent::~MouseButtonPressEvent()
    {
#ifdef DWE_DEBUG
        if (_debug_buffer)
            free(_debug_buffer);
#endif
    }

    MouseButtonReleaseEvent::~MouseButtonReleaseEvent()
    {
#ifdef DWE_DEBUG
        if (_debug_buffer)
            free(_debug_buffer);
#endif
    }

    MouseButtonRepeatEvent::~MouseButtonRepeatEvent()
    {
#ifdef DWE_DEBUG
        if (_debug_buffer)
            free(_debug_buffer);
#endif
    }

    MouseScrollEvent::~MouseScrollEvent()
    {
#ifdef DWE_DEBUG
        if (_debug_buffer)
            free(_debug_buffer);
#endif
    }

}
