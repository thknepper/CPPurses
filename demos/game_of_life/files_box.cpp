#include "files_box.hpp"

#include <string>

#include <cppurses/painter/color.hpp>
#include <cppurses/widget/widgets/confirm_button.hpp>
#include <cppurses/widget/widgets/push_button.hpp>

using namespace cppurses;
namespace gol {

File_widget::File_widget(std::string const& btn_text)
    : confirm_btn_{this->make_child<Confirm_button>(btn_text)}
{
    this->height_policy.maximum(2);
    confirm_btn_.clicked.connect(
        [this]() { filename_given(filename_box_.contents().str()); });
    confirm_btn_.main_btn.brush.set_background(Color::White);
    confirm_btn_.main_btn.brush.set_foreground(Color::Dark_gray);
}

Files_box::Files_box()
{
    this->height_policy.maximum(5);
    this->border.enable();
    this->border.segments.disable_all();
    this->border.segments.north.enable();
    this->border.segments.north = L'─'_g | foreground(Color::Blue);
}

}  // namespace gol
