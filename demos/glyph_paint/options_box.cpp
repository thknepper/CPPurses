#include "options_box.hpp"

#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph.hpp>
#include <cppurses/painter/palettes.hpp>
#include <cppurses/system/system.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

using namespace cppurses;

namespace demos {
namespace glyph_paint {

Options_A::Options_A()
{
    clone_btn.brush.set_background(Color::White);
    clone_btn.brush.set_foreground(Color::Black);

    clear_btn.main_btn.brush.set_background(Color::White);
    clear_btn.main_btn.brush.set_foreground(Color::Black);

    more_btn.brush.set_background(Color::Light_gray);
    more_btn.brush.set_foreground(Color::Black);
}

Options_B::Options_B()
{
    space1.wallpaper = L'─';

    palette_label.set_alignment(Alignment::Center);
    palette_box.add_option("Dawn Bringer").connect([] {
        System::terminal.set_color_palette(Palettes::DawnBringer());
    });
    palette_box.add_option("Standard").connect([] {
        System::terminal.set_color_palette(Palettes::Standard());
    });

    back_btn.brush.set_background(Color::Light_gray);
    back_btn.brush.set_foreground(Color::Black);

    palette_box.brush.set_background(Color::White);
    palette_box.brush.set_foreground(Color::Black);
}

Options_stack::Options_stack()
{
    this->set_active_page(0);
    this->height_policy.maximum(6);

    options_a.more_btn.clicked.connect(
        cppurses::slot::set_active_page(*this, 1));

    options_b.back_btn.clicked.connect(
        cppurses::slot::set_active_page(*this, 0));
}
}  // namespace glyph_paint
}  // namespace demos
