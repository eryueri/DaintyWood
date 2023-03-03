#pragma once

#include "Event.hh"

namespace DWE {
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;
        // ~WindowCloseEvent();

        EVENT_CLASS_TYPE(WindowClose);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Window);

#ifdef DWE_DEBUG
        std::string verbose() override;
#endif

    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent() = delete;
        WindowResizeEvent(int width, int height)
            : _resize_width(width), _resize_height(height) {}
        // ~WindowResizeEvent();

        EVENT_CLASS_TYPE(WindowResize);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Window);

#ifdef DWE_DEBUG
        std::string verbose() override;
#endif

    private:
        int _resize_width, _resize_height;
    };

    class WindowMoveEvent : public Event {
    public:
        WindowMoveEvent() = delete;
        WindowMoveEvent(int x, int y)
            : _move_to_x(x), _move_to_y(y) {}
        // ~WindowMoveEvent();

        EVENT_CLASS_TYPE(WindowMove);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Window);

#ifdef DWE_DEBUG
        std::string verbose()  override;
#endif

    private:
        int _move_to_x, _move_to_y;
    };

    class WindowFocusEvent : public Event {
    public:
        WindowFocusEvent() = default;
        WindowFocusEvent(int focused)
            : _focused(focused) {}
        // ~WindowFocusEvent();

        EVENT_CLASS_TYPE(WindowFocus);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Window);

#ifdef DWE_DEBUG
        std::string verbose() override;
#endif

    private:
        int _focused;
    };
}
