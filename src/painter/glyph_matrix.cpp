#include <cppurses/painter/glyph_matrix.hpp>

#include <cstddef>

#include <cppurses/painter/glyph.hpp>

namespace cppurses {

void Glyph_matrix::resize(std::size_t width, std::size_t height)
{
    matrix_.resize(height);
    matrix_.shrink_to_fit();
    for (auto& row : matrix_) {
        row.resize(width, L' '_g);
        row.shrink_to_fit();
    }
}

}  // namespace cppurses
