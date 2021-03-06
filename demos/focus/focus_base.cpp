#include "focus_base.hpp"

#include <cppurses/system/system.hpp>

namespace {
using namespace cppurses;

auto to_text(Focus_policy policy) -> std::string
{
    switch (policy) {
        case Focus_policy::None: return "None";
        case Focus_policy::Tab: return "Tab";
        case Focus_policy::Click: return "Click";
        case Focus_policy::Strong: return "Strong";
        case Focus_policy::Direct: return "Direct";
    }
    return "";
}

void set_background(Border& border, Color color)
{
    border.segments.north.brush.set_background(color);
    border.segments.south.brush.set_background(color);
    border.segments.east.brush.set_background(color);
    border.segments.west.brush.set_background(color);
    border.segments.north_east.brush.set_background(color);
    border.segments.north_west.brush.set_background(color);
    border.segments.south_east.brush.set_background(color);
    border.segments.south_west.brush.set_background(color);
}

void set_foreground(Border& border, Color color)
{
    border.segments.north.brush.set_foreground(color);
    border.segments.south.brush.set_foreground(color);
    border.segments.east.brush.set_foreground(color);
    border.segments.west.brush.set_foreground(color);
    border.segments.north_east.brush.set_foreground(color);
    border.segments.north_west.brush.set_foreground(color);
    border.segments.south_east.brush.set_foreground(color);
    border.segments.south_west.brush.set_foreground(color);
}

}  // namespace

namespace demos {
namespace focus {

Focus_base::Focus_base(cppurses::Focus_policy policy)
{
    this->set_policy(policy);
    this->border.enable();
}

auto Focus_base::focus_in_event() -> bool
{
    set_background(this->border, cppurses::Color::Red);
    set_foreground(this->border, cppurses::Color::Light_green);
    this->update();
    return Widget::focus_in_event();
}

auto Focus_base::focus_out_event() -> bool
{
    set_background(this->border, cppurses::Color::Black);
    set_foreground(this->border, cppurses::Color::White);
    this->update();
    return Widget::focus_out_event();
}

void Focus_base::set_policy(Focus_policy policy)
{
    title_ = to_text(policy);
    this->focus_policy = policy;
}

}  // namespace focus
}  // namespace demos
