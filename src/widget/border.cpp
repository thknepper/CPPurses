#include <cppurses/widget/border.hpp>

#include <cstddef>

#include <cppurses/painter/glyph.hpp>

namespace cppurses {

void Border::Segments::enable_all()
{
    north.enable();
    south.enable();
    east.enable();
    west.enable();
    north_west.enable();
    north_east.enable();
    south_west.enable();
    south_east.enable();
}

void Border::Segments::disable_all()
{
    north.disable();
    south.disable();
    east.disable();
    west.disable();
    north_west.disable();
    north_east.disable();
    south_west.disable();
    south_east.disable();
}

void Border::Segments::enable_corners()
{
    north_west.enable();
    north_east.enable();
    south_west.enable();
    south_east.enable();
}

void Border::Segments::disable_corners()
{
    north_west.disable();
    north_east.disable();
    south_west.disable();
    south_east.disable();
}

void Border::Segments::enable_sides()
{
    north.enable();
    south.enable();
    east.enable();
    west.enable();
}

void Border::Segments::disable_sides()
{
    north.disable();
    south.disable();
    east.disable();
    west.disable();
}
}  // namespace cppurses
