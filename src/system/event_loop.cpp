#include <cppurses/system/event_loop.hpp>

#include <cppurses/system/detail/event_engine.hpp>
#include <cppurses/system/system.hpp>

namespace cppurses {

auto Event_loop::run() -> int
{
    if (running_)
        return -1;
    running_    = true;
    auto notify = true;
    while (not exit_) {
        if (notify)
            detail::Event_engine::get().notify();
        if (is_main_thread_)
            detail::Event_engine::get().process();
        notify = this->loop_function();
    }
    running_ = false;
    exit_    = false;
    return return_code_;
}

void Event_loop::connect_to_system_exit()
{
    auto exit_loop =
        sig::Slot<void(int)>{[this](int code) { this->exit(code); }};
    exit_loop.track(lifetime_);
    System::exit_signal.connect(exit_loop);
}

}  // namespace cppurses
