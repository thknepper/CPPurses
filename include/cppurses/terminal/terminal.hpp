#ifndef CPPURSES_TERMINAL_TERMINAL_HPP
#define CPPURSES_TERMINAL_TERMINAL_HPP
#include <chrono>
#include <cstddef>

#include <cppurses/painter/glyph.hpp>
#include <cppurses/painter/palette.hpp>
#include <cppurses/painter/palettes.hpp>

namespace cppurses {

class Terminal {
   public:
    /// Initializes the terminal screen into curses mode.
    /** Must be called before any input/output can occur. Also initializes
     *  various proerties that are modifiable from this Terminal class. No-op if
     *  already initialized. */
    void initialize();

    /// Reset the terminal to its state before initialize() was called.
    /** No-op if already uninitialized. */
    void uninitialize();

    /// Return the width of the terminal screen.
    auto width() const -> std::size_t;

    /// Return the height of the terminal screen.
    auto height() const -> std::size_t;

    /// Set the rate at which the screen will update.
    /** User input will immediately flush, but other event loops are only
     *  processed every refresh rate. Default is 33ms. */
    void set_refresh_rate(std::chrono::milliseconds duration);

    /// Set the default background/wallpaper tiles to be used.
    /** This is used if a Widget has no assigned wallpaper. */
    void set_background(Glyph const& tile);

    /// Return the default background/wallpaper currently in use.
    auto background() const -> Glyph const& { return background_; }

    /// Set terminal color definitions for the 16 Colors in CPPurses.
    void set_color_palette(Palette const& colors);

    /// Return a copy of the currently set color palette.
    auto current_palette() const -> Palette { return palette_; }

    /// Set whether or not the cursor is visible on screen.
    void show_cursor(bool show = true);

    /// Enable or disable raw mode.
    /** In raw mode, the interrupt, quit, suspend, and flow control characters
     *  are all passed through uninterpreted, instead of generating a signal.
     *  This mode is off by default. */
    void raw_mode(bool enable = true);

    /// Return whether this terminal can display colors.
    /** Return false if the Terminal hasn't been initialized yet. */
    auto has_color() const -> bool;

    /// Return whether this terminal can display up to 16 colors.
    auto has_extended_colors() const -> bool;

    /// Return the number of colors this terminal supports.
    auto color_count() const -> short;

    /// Return whether this terminal can change color definitions.
    /** This is required for the color Palette to be changed. Return false if
     *  the Terminal hasn't been initialized yet. */
    auto can_change_colors() const -> bool;

    /// Return the number of color pairs in this terminal.
    /** Always returns 0 ...*/
    auto color_pair_count() const -> short;

    /// Map pairs of colors to a unique index between [0, 255]
    auto color_index(short fg, short bg) const -> short;

    /// Use the default colors of the terminal for bg Black and fg White.
    void use_default_colors(bool use = true);

   private:
    bool is_initialized_ = false;
    bool show_cursor_    = false;
    bool raw_mode_       = false;
    Glyph background_    = L' ';
    Palette palette_     = Palettes::DawnBringer();
    std::chrono::milliseconds refresh_rate_{33};

    /// Actually set the palette via ncurses using the state of \p colors.
    void ncurses_set_palette(Palette const& colors);

    /// Actually set raw/noraw mode via ncurses using the state of raw_mode_.
    void ncurses_set_raw_mode() const;

    /// Actually set show_cursor via ncurses using the state of show_cursor_.
    void ncurses_set_cursor() const;

    /// Assign all color pairs.
    void initialize_color_pairs() const;

    /// Assign background Black and foreground White as color -1
    void init_default_pairs() const;

    /// Assign background Black and foreground White as color 0 and 7 resp.
    void uninit_default_pairs() const;
};

}  // namespace cppurses
#endif  // CPPURSES_TERMINAL_TERMINAL_HPP
