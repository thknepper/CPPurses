#ifndef CPPURSES_DEMOS_PALETTE_PALETTE_DEMOS_HPP
#define CPPURSES_DEMOS_PALETTE_PALETTE_DEMOS_HPP
#include <cppurses/widget/layouts/horizontal.hpp>

#include "all_colors_display.hpp"
#include "color_definition_setter.hpp"

namespace palette {

/// Provides a user interface to the color palette system.
class Palette_demo : public cppurses::layout::Horizontal<> {
   public:
    Palette_demo()
    {
        all_colors_display_.color_select.color_selected.connect(
            [this](cppurses::Color c) {
                color_setter_.change_current_color(c);
            });
    }

   private:
    Color_definition_setter& color_setter_ =
        this->make_child<Color_definition_setter>();
    All_colors_display& all_colors_display_ =
        this->make_child<All_colors_display>();
};

}  // namespace palette
#endif  // CPPURSES_DEMOS_PALETTE_PALETTE_DEMOS_HPP
