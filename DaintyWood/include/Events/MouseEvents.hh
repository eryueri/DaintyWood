#pragma once

#include "Event.hh"

namespace DWE {
    class MouseMoveEvent : public Event {
    public:
        MouseMoveEvent() = delete;
        MouseMoveEvent(float xpos, float ypos)
            : _x_pos(xpos), _y_pos(ypos) {}
        // ~MouseMoveEvent();

        EVENT_CLASS_TYPE(MouseMove);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse);

#ifdef DWE_DEBUG
        std::string verbose() override;
#endif

    private:
        float _x_pos, _y_pos;
    };

    class MouseButtonPressEvent : public Event {
    public:
        MouseButtonPressEvent() = delete;
        MouseButtonPressEvent(int code)
            : _code(code) {}
        // ~MouseButtonPressEvent();

        EVENT_CLASS_TYPE(MouseButtonPress);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse);

#ifdef DWE_DEBUG
        std::string verbose() override;
#endif

    private:
        int _code;
    };

    class MouseButtonReleaseEvent : public Event {
    public:
        MouseButtonReleaseEvent() = delete;
        MouseButtonReleaseEvent(int code)
            : _code(code) {}
        // ~MouseButtonReleaseEvent();

        EVENT_CLASS_TYPE(MouseButtonRelease);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse);

#ifdef DWE_DEBUG
        std::string verbose() override;
#endif

    private:
        int _code;
    };

    class MouseButtonRepeatEvent : public Event {
    public:
        MouseButtonRepeatEvent() = delete;
        MouseButtonRepeatEvent(int code)
            : _code(code) {}
        // ~MouseButtonRepeatEvent();

        EVENT_CLASS_TYPE(MouseButtonRepeat);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse);

#ifdef DWE_DEBUG
        std::string verbose() override;
#endif

    private:
        int _code;
    };

    class MouseScrollEvent : public Event {
    public:
        MouseScrollEvent() = delete;
        MouseScrollEvent(float xoffset, float yoffset)
            : _x_offset(xoffset), _y_offset(yoffset) {}
        // ~MouseScrollEvent();

        EVENT_CLASS_TYPE(MouseScroll);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse);

#ifdef DWE_DEBUG
        std::string verbose() override;
#endif

    private:
        float _x_offset, _y_offset;
    };

}
