#ifndef SYSTEM_MODULE_EVENTS_CLOSE_EVENT_HPP
#define SYSTEM_MODULE_EVENTS_CLOSE_EVENT_HPP

#include "system_module/event.hpp"

namespace cppurses {

class Close_event : public Event {
   public:
    Close_event() : Event{Event::Close} {}
};

}  // namespace cppurses
#endif  // SYSTEM_MODULE_EVENTS_CLOSE_EVENT_HPP