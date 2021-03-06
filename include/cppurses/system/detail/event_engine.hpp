#ifndef CPPURSES_SYSTEM_DETAIL_EVENT_ENGINE_HPP
#define CPPURSES_SYSTEM_DETAIL_EVENT_ENGINE_HPP
#include <atomic>
#include <memory>

#include <cppurses/painter/detail/screen.hpp>
#include <cppurses/painter/detail/staged_changes.hpp>
#include <cppurses/system/detail/event_queue.hpp>
#include <cppurses/system/event.hpp>
#include <cppurses/system/system.hpp>

namespace cppurses {
namespace detail {

/// Orchestrates all event processing and queueing.
class Event_engine {
   public:
    /// Event_loop uses this to let main thread know it should process events.
    void notify() { notified_ = true; }

    /// Invokes events and flush the screen.
    void process()
    {
        invoke_events(queue_);
        flush_screen();
    }

    /// Return a reference to the internal Event_queue.
    auto queue() -> Event_queue& { return queue_; }

    /// Get the global Event_engine instantiation.
    static auto get() -> Event_engine&
    {
        static Event_engine engine;
        return engine;
    }

   private:
    Event_engine() = default;

    /// Flushes all of the staged changes to the screen and sets the cursor.
    static void flush_screen()
    {
        auto& staged_changes = Staged_changes::get();
        Screen::flush(staged_changes);
        staged_changes.clear();
        Screen::set_cursor_on_focus_widget();
    }

    /// Send all \p type events in queue to their Widgets.
    template <Event::Type filter>
    static void send_all(Event_queue& queue)
    {
        for (std::unique_ptr<Event> event : Event_queue::View<filter>{queue}) {
            System::send_event(*event);
        }
    }

    /// Send all delete events to their Widgets.
    /** Removes any events to the receiver if another thread posted them. */
    static void send_all_deletes(Event_queue& queue)
    {
        auto view = Event_queue::View<Event::Delete>{queue};
        for (std::unique_ptr<Event> event : view) {
            Widget* receiver = &(event->receiver());
            System::send_event(*event);
            queue.remove_events_of(receiver);
        }
    }

    /// Sends each event in \p queue to its receiver to be processed.
    static void invoke_events(Event_queue& queue)
    {
        send_all<Event::None>(queue);
        send_all<Event::Paint>(queue);
        send_all_deletes(queue);
        queue.clean();
    }

   private:
    Event_queue queue_;
    std::atomic<bool> notified_{false};  // Whether should process events/flush
};

}  // namespace detail
}  // namespace cppurses
#endif  // CPPURSES_SYSTEM_DETAIL_EVENT_ENGINE_HPP
