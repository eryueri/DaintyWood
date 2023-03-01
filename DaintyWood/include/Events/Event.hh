#pragma once

#include "pch.hh"

#define BIT(x) 1 << x

namespace DWE {
    enum class EventType {
        None = 0,
        KeyPress, KeyRelease, KeyRepeat, 
        MouseMove, MouseButtonPress, MouseButtonRelease, MouseButtonRepeat, MouseScroll, 
        ControllerButtonPress, ControllerButtonReleas, ConttrollerButtonRepeat, 
        WindowClose, WindowResize, WindowMove, WindowFocus,
    };

    enum class EventCategory : uint8_t {
        None = 0,
        Input = BIT(0),
        Output = BIT(1),
        Keyboard = BIT(2),
        Mouse = BIT(3),
        Controller = BIT(4),
        Window = BIT(5),
    };

    class Event {
    public:
        virtual EventType getEventType() const = 0;
        virtual uint8_t getEventCategoryFlags() const = 0;

        bool isInCategory(const EventCategory& category);

#ifdef DWE_DEBUG
    public:
        virtual const char* verbose() = 0;
        char* _debug_buffer = nullptr;
#endif
    };

    uint8_t operator| (const EventCategory& a, const EventCategory& b);
    uint8_t operator| (const uint8_t& a, const EventCategory& b);
    uint8_t operator| (const EventCategory& a, const uint8_t& b);
    uint8_t operator& (const EventCategory& a, const EventCategory& b);
    uint8_t operator& (const uint8_t& a, const EventCategory& b);
    uint8_t operator& (const EventCategory& a, const uint8_t& b);

#define EVENT_CLASS_TYPE(type) \
    static EventType getStaticType() { return EventType::type; } \
    EventType getEventType() const override { return getStaticType(); }
#define EVENT_CLASS_CATEGORY(category) \
    uint8_t getEventCategoryFlags() const override { return category; }

}

#undef BIT
