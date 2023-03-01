#pragma once

#include "Event.hh"

namespace DWE {
    class KeyPressEvent : public Event {
    public:
        KeyPressEvent() = delete;
        KeyPressEvent(int code) : _code(code) {}
        ~KeyPressEvent();

        EVENT_CLASS_TYPE(KeyPress);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Keyboard);
        
#ifdef DWE_DEBUG
        const char* verbose() override;
#endif
        
    private:
        int _code;
    };

    class KeyReleaseEvent : public Event {
    public:
        KeyReleaseEvent() = delete;
        KeyReleaseEvent(int code) : _code(code) {}
        ~KeyReleaseEvent();

        EVENT_CLASS_TYPE(KeyRelease);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Keyboard);
        
#ifdef DWE_DEBUG
        const char* verbose() override;
#endif
        
    private:
        int _code;
    };

    class KeyRepeatEvent : public Event {
    public:
        KeyRepeatEvent() = delete;
        KeyRepeatEvent(int code) : _code(code) {}
        ~KeyRepeatEvent();

        EVENT_CLASS_TYPE(KeyRepeat);
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Keyboard);
        
#ifdef DWE_DEBUG
        const char* verbose() override;
#endif
        
    private:
        int _code;
    };
}
