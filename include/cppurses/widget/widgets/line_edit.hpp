#ifndef CPPURSES_WIDGET_WIDGETS_LINE_EDIT_HPP
#define CPPURSES_WIDGET_WIDGETS_LINE_EDIT_HPP
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include <signals/signals.hpp>

#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph.hpp>
#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/system/events/key.hpp>
#include <cppurses/system/events/mouse.hpp>
#include <cppurses/widget/widgets/textbox.hpp>

namespace cppurses {
struct Point;

// TODO input_mask option, which shows outline of acceptable text in ghost color
// ex) phone #| (___)___-____
// ip address| ___.___.___.___
// ref: webtoolkit.eu/widgets/forms/line_text-editor

// TODO change to class Field?
/// Text input box with input validator and Signal emitted on Enter Key press.
/** Initial text is in ghost color and cleared on initial focus. Height is fixed
 *  at 1. */
class Line_edit : public Textbox {
   public:
    using Validator_t = std::function<bool(char)>;

    /// Emitted on Enter Key press, sends along the current contents.
    sig::Signal<void(std::string)> edit_finished;

   public:
    /// Construct a Line_edit object with \p initial_text in ghost color.
    explicit Line_edit(Glyph_string initial_text = "")
        : Textbox{std::move(initial_text)}
    {
        this->set_ghost_color(Color::Light_gray);
        this->height_policy.fixed(1);
        this->disable_word_wrap();
    }

    /// Set the input validator, allowing or disallowing specific char types.
    /** The default validator always returns true, allowing all chars. Invalid
     *  character input will result in no input being given to the Line_edit. */
    void set_validator(Validator_t validator)
    {
        validator_ = std::move(validator);
    }

    /// Set whether the text is cleared from the Line_edit on Enter Key press.
    /** This is disabled by default. */
    void clear_on_enter(bool enable = true) { clear_on_enter_ = enable; }

    /// Set whether the Line_edit has veilded output, doesn't alter the content.
    /** Disabled by default, uses '*' to veil by default. */
    void veil_text(bool enable = true)
    {
        is_veiled_ = enable;
        this->update();
    }

    /// Set Glyph used to obscure the display.
    void set_veil(Glyph const& veil)
    {
        veil_ = veil;
        this->update();
    }

    /// Set whether the Line_edit has an underline.
    /** Disabled by default. The entire length of the box is underlined if
     *  enabled. */
    void underline(bool enabled = true);

    /// Set color of the initial text, before focus has been given to this.
    void set_ghost_color(Color c);

   protected:
    auto key_press_event(Key::State const& keyboard) -> bool override;

    auto mouse_press_event(Mouse::State const& mouse) -> bool override
    {
        switch (mouse.button) {
            case Mouse::Button::ScrollUp:
            case Mouse::Button::ScrollDown: return true;
            default: return Textbox::mouse_press_event(mouse);
        }
    }

    auto focus_in_event() -> bool override
    {
        if (on_initial_)
            this->clear();
        return Textbox::focus_in_event();
    }

    auto paint_event() -> bool override
    {
        if (is_veiled_) {
            auto const vc = std::vector<Glyph>(this->contents().size(), veil_);
            this->set_contents(Glyph_string{std::begin(vc), std::end(vc)});
        }
        return Textbox::paint_event();
    }

   private:
    bool clear_on_enter_   = false;
    bool on_initial_       = true;
    bool is_veiled_        = false;
    Glyph veil_            = L'*';
    Validator_t validator_ = [](char) { return true; };
};
}  // namespace cppurses
#endif  // CPPURSES_WIDGET_WIDGETS_LINE_EDIT_HPP
