#include <cppurses/widget/detail/border_offset.hpp>

#include <cstddef>

#include <cppurses/widget/border.hpp>
#include <cppurses/widget/widget.hpp>

namespace cppurses {
namespace detail {

auto Border_offset::west_disqualified(Widget const& w) -> bool
{
    return w.outer_width() <= 1;
}

auto Border_offset::east_disqualified(Widget const& w) -> bool
{
    if (w.outer_width() <= 1)
        return true;
    if (w.outer_width() == 2 and w.border.segments.west.enabled())
        return true;
    return false;
}

auto Border_offset::north_disqualified(Widget const& w) -> bool
{
    return w.outer_height() <= 1;
}

auto Border_offset::south_disqualified(Widget const& w) -> bool
{
    if (w.outer_height() <= 1)
        return true;
    if (w.outer_height() == 2 and w.border.segments.north.enabled())
        return true;
    return false;
}

auto Border_offset::west(Widget const& w) -> std::size_t
{
    auto const& b = w.border.segments;
    if (w.border.enabled() and !west_disqualified(w) and
        (b.west.enabled() or b.north_west.enabled() or
         b.south_west.enabled())) {
        return 1;
    }
    return 0;
}

auto Border_offset::east(Widget const& w) -> std::size_t
{
    auto const& b = w.border.segments;
    if (w.border.enabled() and !east_disqualified(w) and
        (b.east.enabled() or b.north_east.enabled() or
         b.south_east.enabled())) {
        return 1;
    }
    return 0;
}

auto Border_offset::north(Widget const& w) -> std::size_t
{
    auto const& b = w.border.segments;
    if (w.border.enabled() and !north_disqualified(w) and
        (b.north.enabled() or b.north_east.enabled() or
         b.north_west.enabled())) {
        return 1;
    }
    return 0;
}

auto Border_offset::south(Widget const& w) -> std::size_t
{
    auto const& b = w.border.segments;
    if (w.border.enabled() and !south_disqualified(w) and
        (b.south.enabled() or b.south_east.enabled() or
         b.south_west.enabled())) {
        return 1;
    }
    return 0;
}

}  // namespace detail
}  // namespace cppurses
