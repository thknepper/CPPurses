#ifndef CPPURSES_WIDGET_WIDGETS_GRAPH_HPP
#define CPPURSES_WIDGET_WIDGETS_GRAPH_HPP
#include <cmath>
#include <cstddef>
#include <iterator>
#include <map>
#include <type_traits>
#include <vector>

#include <cppurses/painter/glyph.hpp>
#include <cppurses/painter/painter.hpp>
#include <cppurses/widget/point.hpp>
#include <cppurses/widget/widget.hpp>

namespace cppurses {

template <typename Number_t = double>
class Graph : public cppurses::Widget {
   public:
    struct Coordinates {
        Number_t x;
        Number_t y;
    };

    // Half Open Range [west, east) [north, south)
    struct Boundary {
        Number_t west  = 0;
        Number_t east  = 0;
        Number_t north = 0;
        Number_t south = 0;
    };

   public:
    Graph(Boundary const& b = {}) : boundary_{b} {}

    template <typename Range_t>
    void add(Range_t const& values)
    {
        this->add(std::begin(values), std::end(values));
    }

    template <typename Iter_t>
    void add(Iter_t first, Iter_t last)
    {
        static_assert(
            std::is_same_v<typename std::iterator_traits<Iter_t>::value_type,
                           Coordinates>,
            "Must add with a container of Coordinates.");
        for (; first != last; ++first) {
            this->add(*first);
        }
    }

    void add(Coordinates c)
    {
        coordinates_.push_back(c);
        this->initialize_bitmap(c);
    }

    void set_boundary(Boundary const& b)
    {
        boundary_ = b;
        this->regenerate_map();
    }

    void clear()
    {
        coordinates_.clear();
        map_.clear();
        this->update();
    }

    // void set_anchors(Number_t north, Number_t south, Number_t east, ...west);
    // // change the resolution by scaling the anchors, maybe have a scale
    // helper
    // // method to do this by a multiple.

    // void scale(Number_t multiple) // multiply each anchor by this.
    // // above one will zoom out, below one will zoom in.

    // void scroll_horizontal(Number_t distance) (should be able to be negative
    // even if number_t is unsigned... or create four overloads, that is better)
    // // this scroll will add the value distance to east and west
   protected:
    auto paint_event() -> bool override
    {
        // Only relies on map_, call regenerate_map() if you need a new size
        cppurses::Painter p{*this};
        for (auto [point, bitmap] : map_) {
            p.put(to_symbol(bitmap), point);
        }
        return Widget::paint_event();
    }

    auto resize_event(Area new_size, Area old_size) -> bool override
    {
        this->regenerate_map();
        return Widget::resize_event(new_size, old_size);
    }

   private:
    class Bitmap {
       public:
        void clear() { bits_ = std::byte{0}; }

        void set_top_left() { bits_ |= std::byte{0b00000001}; }
        void set_top_right() { bits_ |= std::byte{0b00001000}; }
        void set_mid_top_left() { bits_ |= std::byte{0b00000010}; }
        void set_mid_top_right() { bits_ |= std::byte{0b00010000}; }
        void set_mid_bottom_left() { bits_ |= std::byte{0b00000100}; }
        void set_mid_bottom_right() { bits_ |= std::byte{0b00100000}; }
        void set_bottom_left() { bits_ |= std::byte{0b01000000}; }
        void set_bottom_right() { bits_ |= std::byte{0b10000000}; }

        auto get() const -> unsigned char
        {
            return std::to_integer<unsigned char>(bits_);
        }

       private:
        std::byte bits_{0};
    };

   private:
    std::map<cppurses::Point, Bitmap> map_;
    std::vector<Coordinates> coordinates_;
    Boundary boundary_;

   private:
    static auto to_symbol(Bitmap b) -> cppurses::Glyph
    {
        auto constexpr initial_braille = L'⠀';
        return wchar_t{initial_braille + b.get()};
    }

    static auto distance(Number_t smaller, Number_t larger) -> Number_t
    {
        return larger - smaller;
    }

    /// User provided Coordinates to visual cppurses::Point on Widget.
    void initialize_bitmap(Coordinates c)
    {
        auto const visual_width  = this->width();
        auto const visual_height = this->height();
        if (visual_width == 0 or visual_height == 0)
            return;
        auto const horizontal_offset = [this, &c, visual_width] {
            auto const domain_width = distance(boundary_.west, boundary_.east);
            auto const interval     = domain_width / visual_width;
            // TODO make interval (h/v) a class member updated on resize or
            // boundary change.
            if (interval == 0)
                return Number_t{0};
            return distance(boundary_.west, c.x) / interval;
        }();
        auto const vertical_offset = [this, &c, visual_height] {
            auto const domain_height =
                distance(boundary_.north, boundary_.south);
            auto const interval = domain_height / visual_height;
            if (interval == 0)
                return Number_t{0};
            auto const distance_from_bottom =
                distance(boundary_.north, c.y) / interval;
            return visual_height - distance_from_bottom;  // Invert
        }();
        auto& bitmap = map_[{static_cast<std::size_t>(horizontal_offset),
                             static_cast<std::size_t>(vertical_offset)}];

        auto const h_dec = std::fmod(horizontal_offset, 1.);
        auto const v_dec = std::fmod(vertical_offset, 1.);
        if (h_dec < 0.5) {
            if (v_dec < 0.25)
                bitmap.set_top_left();
            else if (v_dec < 0.5)
                bitmap.set_mid_top_left();
            else if (v_dec < 0.75)
                bitmap.set_mid_bottom_left();
            else
                bitmap.set_bottom_left();
        }
        else {
            if (v_dec < 0.25)
                bitmap.set_top_right();
            else if (v_dec < 0.5)
                bitmap.set_mid_top_right();
            else if (v_dec < 0.75)
                bitmap.set_mid_bottom_right();
            else
                bitmap.set_bottom_right();
        }
    }

    void regenerate_map()
    {
        map_.clear();
        for (auto coord : coordinates_) {
            this->initialize_bitmap(coord);
        }
    }
};

}  // namespace cppurses
#endif  // CPPURSES_WIDGET_WIDGETS_GRAPH_HPP
