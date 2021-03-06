#ifndef CPPURSES_WIDGET_WIDGETS_CYCLE_STACK_HPP
#define CPPURSES_WIDGET_WIDGETS_CYCLE_STACK_HPP
#include <memory>
#include <type_traits>
#include <utility>

#include <signals/signal.hpp>

#include <cppurses/painter/attribute.hpp>
#include <cppurses/painter/brush.hpp>
#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/layouts/stack.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widget.hpp>
#include <cppurses/widget/widgets/cycle_box.hpp>
#include <cppurses/widget/widgets/push_button.hpp>

namespace cppurses {

/// A layout::Stack with an interface to cycle through each Widget in the stack.
template <typename Child_t = Widget>
class Cycle_stack : public layout::Vertical<Widget> {
   private:
    /// User interface to cycle through the pages of the Stack.
    class Top_row : public layout::Horizontal<Widget> {
       public:
        Push_button& left_btn  = this->make_child<Push_button>("<");
        Cycle_box& cycle_box   = this->make_child<Cycle_box>();
        Push_button& right_btn = this->make_child<Push_button>(">");

       public:
        Top_row()
        {
            this->height_policy.fixed(1);
            left_btn.width_policy.fixed(1);
            left_btn.clicked.connect(slot::previous(cycle_box));

            right_btn.width_policy.fixed(1);
            right_btn.clicked.connect(slot::next(cycle_box));

            cycle_box.brush.add_attributes(Attribute::Bold);

            for (auto& child : this->get_children()) {
                child.brush.set_background(Color::Light_gray);
                child.brush.set_foreground(Color::Black);
            }
        }
    };

   public:
    Top_row& top_row              = this->make_child<Top_row>();
    layout::Stack<Child_t>& stack = this->make_child<layout::Stack<Child_t>>();

   public:
    /// Construct a new Widget_t object and add it to the end of the Stack.
    /** Returns a reference to this newly created page. \p title is passed to
     *  the Cycle_box to display when this page is active. */
    template <typename Widget_t = Child_t, typename... Args>
    auto make_page(Glyph_string title, Args&&... args) -> Widget_t&
    {
        static_assert(
            std::is_base_of<Child_t, Widget_t>::value,
            "Cycle_stack::make_page: Widget_t must be a Child_t type");
        auto child = std::make_unique<Widget_t>(std::forward<Args>(args)...);
        return static_cast<Widget_t&>(
            this->append_page(std::move(title), std::move(child)));
    }

    /// Append a page to the Stack.
    /** \p title is passed to the Cycle_box associated with this page. */
    auto append_page(Glyph_string title, std::unique_ptr<Child_t> widget)
        -> Child_t&
    {
        auto& signal = top_row.cycle_box.add_option(std::move(title));
        signal.connect(slot::set_active_page(stack, stack.size()));
        auto& child = stack.append_page(std::move(widget));
        if (stack.size() == 1)
            stack.set_active_page(0);
        return child;
    }
};

}  // namespace cppurses
#endif  // CPPURSES_WIDGET_WIDGETS_CYCLE_STACK_HPP
