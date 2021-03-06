#ifndef CPPURSES_WIDGET_WIDGETS_STATUS_HPP
#define CPPURSES_WIDGET_WIDGETS_STATUS_HPP
#include <utility>

#include <signals/signals.hpp>

#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/widget/widgets/label.hpp>

namespace cppurses {

class Status_bar : public Label {
   public:
    sig::Signal<void(Glyph_string const&)> status_updated;

   public:
    explicit Status_bar(Glyph_string initial_message = "")
        : Label{std::move(initial_message)}
    {}

    void update_status(Glyph_string message)
    {
        this->set_contents(std::move(message));
        status_updated(this->contents());
    }
};

namespace slot {

inline auto update_status(Status_bar& sb) -> sig::Slot<void(Glyph_string)>
{
    auto slot = sig::Slot<void(Glyph_string)>{
        [&sb](Glyph_string message) { sb.update_status(std::move(message)); }};
    slot.track(sb.destroyed);
    return slot;
}

inline auto update_status(Status_bar& sb, Glyph_string const& message)
    -> sig::Slot<void()>
{
    auto slot =
        sig::Slot<void()>{[&sb, message] { sb.update_status(message); }};
    slot.track(sb.destroyed);
    return slot;
}

}  // namespace slot
}  // namespace cppurses
#endif  // CPPURSES_WIDGET_WIDGETS_STATUS_HPP
