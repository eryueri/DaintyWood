#include "Events/WindowEvents.hh"

namespace DWE {
#ifdef DWE_DEBUG
    const char* WindowCloseEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*20);
        sprintf(_debug_buffer, "Window is closed!");
        return _debug_buffer;
    }

    const char* WindowResizeEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*36);
        sprintf(_debug_buffer, "Window is Resized to: %d, %d", _resize_width, _resize_height);
        return _debug_buffer;
    }

    const char* WindowMoveEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*36);
        sprintf(_debug_buffer, "Window is Moved to: %d, %d", _move_to_x, _move_to_y);
        return _debug_buffer;
    }

    const char* WindowFocusEvent::verbose()
    {
        _debug_buffer = (char*)malloc(sizeof(char)*24);
        if (_focused)
            sprintf(_debug_buffer, "Window is focused!");
        else
            sprintf(_debug_buffer, "Window is unfocused!");
        return _debug_buffer;
    }

#endif
    WindowCloseEvent::~WindowCloseEvent()
    {
#ifdef DWE_DEBUG
        if (_debug_buffer)
            free(_debug_buffer);
#endif
    }

    WindowResizeEvent::~WindowResizeEvent()
    {
#ifdef DWE_DEBUG
        if (_debug_buffer)
            free(_debug_buffer);
#endif
    }

    WindowMoveEvent::~WindowMoveEvent()
    {
#ifdef DWE_DEBUG
        if (_debug_buffer)
            free(_debug_buffer);
#endif
    }

    WindowFocusEvent::~WindowFocusEvent()
    {
#ifdef DWE_DEBUG
        if (_debug_buffer)
            free(_debug_buffer);
#endif
    }

}
