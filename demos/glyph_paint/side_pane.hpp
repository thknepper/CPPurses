#ifndef DEMOS_GLYPH_PAINT_SIDE_PANE_HPP
#define DEMOS_GLYPH_PAINT_SIDE_PANE_HPP
#include "attribute_box.hpp"
#include "options_box.hpp"
#include "populated_glyph_stack.hpp"

#include <cppurses/painter/attribute.hpp>
#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/color_select.hpp>
#include <cppurses/widget/widgets/cycle_stack.hpp>
#include <cppurses/widget/widgets/fixed_height.hpp>
#include <cppurses/widget/widgets/status_bar.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

namespace demos {
namespace glyph_paint {

class Side_pane : public cppurses::layout::Vertical<> {
    struct Color_pages : cppurses::Cycle_stack<cppurses::Color_select> {
        Color_pages() { this->height_policy.maximum(3); }
        cppurses::Color_select& foreground{this->make_page(
            cppurses::Glyph_string{"Foreground", cppurses::Attribute::Bold})};
        cppurses::Color_select& background{this->make_page(
            cppurses::Glyph_string{"Background", cppurses::Attribute::Bold})};
    };

   public:
    Side_pane()
    {
        this->width_policy.fixed(16);
        space1.wallpaper = L'─';
        space2.wallpaper = L'─';
        glyph_select.height_policy.preferred(6);
        show_glyph.height_policy.fixed(1);
        show_glyph.set_alignment(cppurses::Alignment::Center);
    }

    Populated_glyph_stack& glyph_select{
        this->make_child<Populated_glyph_stack>()};

    cppurses::Fixed_height& space1{this->make_child<cppurses::Fixed_height>(1)};

    Color_pages& color_pages{this->make_child<Color_pages>()};

    Attribute_box& attribute_box{this->make_child<Attribute_box>()};

    cppurses::Status_bar& show_glyph{
        this->make_child<cppurses::Status_bar>("x")};

    cppurses::Fixed_height& space2{this->make_child<cppurses::Fixed_height>(1)};

    Options_stack& options_box{this->make_child<Options_stack>()};
};

}  // namespace glyph_paint
}  // namespace demos
#endif  // DEMOS_GLYPH_PAINT_SIDE_PANE_HPP
