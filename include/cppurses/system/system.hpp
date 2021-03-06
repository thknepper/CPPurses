#ifndef CPPURSES_SYSTEM_SYSTEM_HPP
#define CPPURSES_SYSTEM_SYSTEM_HPP
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include <signals/signal.hpp>
#include <signals/slot.hpp>

#include <cppurses/system/detail/user_input_event_loop.hpp>
#include <cppurses/system/event.hpp>
#include <cppurses/terminal/terminal.hpp>

namespace cppurses {
class Animation_engine;
class Widget;

/// Organizes the highest level of the TUI framework.
/** Constructing an instance of this class initializes the display system.
 *  Manages the head Widget and the main User_input_event_loop. */
class System {
   public:
    /// Emitted when System::exit is called. Should call Event_loop::exit.
    /** Passes along the exit_code System::exit() was called with. */
    static sig::Signal<void(int)> exit_signal;

    // Slots
    static sig::Slot<void()> quit;

    /// Provides access to and modification of global terminal properties.
    static Terminal terminal;

   public:
    System()              = default;
    System(System const&) = delete;
    System& operator=(System const&) = delete;
    System(System&&)                 = default;
    System& operator=(System&&) = default;

    ~System() { System::exit(0); }

    /// Return a pointer to the currently focused Widget.
    static auto focus_widget() -> cppurses::Widget*;

    /// Give program focus to \p w.
    /** Sends Focus_out_event to Widget in focus, and Focus_in_event to \p w.*/
    static void set_focus(Widget& w);

    /// Removes focus from the currently in focus Widget.
    static void clear_focus();

    /// Enable Tab/Back_tab keys to change the focus Widget.
    static void enable_tab_focus();

    /// Disable Tab/Back_tab keys from changing focus Widget.
    static void disable_tab_focus();

    /// Set a new head Widget for the entire system.
    /** Will disable the previous head widget if not nullptr. Only valid to call
     *  before System::run or after System::exit. */
    static void set_head(Widget* new_head);

    /// Return a pointer to the head Widget.
    /** This Widget is the ancestor of every other widget that will be displayed
     *  on the screen. */
    static auto head() -> Widget* { return head_; }

    /// Create a Widget_t object, set it as head widget and call System::run().
    /** \p args... are passed on to the Widget_t constructor. Blocks until
     *  System::exit() is called, returns the exit code. Will throw a
     *  std::runtime_error if screen cannot be initialized. */
    template <typename Widget_t, typename... Args>
    auto run(Args&&... args) -> int
    {
        auto head = Widget_t(std::forward<Args>(args)...);
        System::set_head(&head);
        return this->run();
    }

    /// Set \p head as head widget and call System::run().
    /** Will throw a std::runtime_error if screen cannot be initialized. */
    auto run(Widget& head) -> int
    {
        System::set_head(&head);
        return this->run();
    }

    /// Launch the main Event_loop and start processing Events.
    /** Blocks until System::exit() is called, returns the exit code. Will throw
     *  a std::runtime_error if screen cannot be initialized. Enables and sets
     *  focus to the head Widget.*/
    static auto run() -> int;

    /// Immediately send the event filters and then to the intended receiver.
    static auto send_event(Event const& event) -> bool;

    /// Append the event to the Event_queue for the thread it was called on.
    /** The Event_queue is processed once per iteration of the Event_loop. When
     *  the Event is pulled from the Event_queue, it is processed by
     *  System::send_event() */
    static void post_event(std::unique_ptr<Event> event);

    /// Append a newly created Event of type T onto the Event_queue.
    /** \p args... are passed onto the constructor of T. Has same behavior as
     *  the non-templated function of the same name once the object has been
     *  constructed. */
    template <typename T, typename... Args>
    static void post_event(Args&&... args)
    {
        auto event = std::make_unique<T>(std::forward<Args>(args)...);
        System::post_event(std::move(event));
    }

    /// Send an exit signal to each of the currently running Event_loops.
    /** Also call shutdown() on the Animation_engine and set
     *  System::exit_requested_ to true. Though it sends the exit signal to each
     *  of the Event_loops, they are not guaranteed to be stopped by the time
     *  this function returns. */
    static void exit(int exit_code = 0);

    /// Return a reference to the Animation_engine in System.
    /** This manages animation on each of the Widgets that enables it. */
    static auto animation_engine() -> Animation_engine&
    {
        return animation_engine_;
    }

    /// Return whether System has gotten an exit request, set by System::exit()
    static auto exit_requested() -> bool { return exit_requested_; }

   private:
    static Widget* head_;
    static bool exit_requested_;
    static detail::User_input_event_loop user_input_loop_;
    static Animation_engine animation_engine_;
};

}  // namespace cppurses
#endif  // CPPURSES_SYSTEM_SYSTEM_HPP
