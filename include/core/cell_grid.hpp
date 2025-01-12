#ifndef MSWPR_CELL_GRID_HPP
#define MSWPR_CELL_GRID_HPP

#include "core/cell.hpp"
#include "core/mines_generator.hpp"

#include <cstddef>
#include <vector>

namespace mswpr
{
  class cell_grid
  {
  public:
    using iterator = std::vector<cell>::iterator;
    using const_iterator = std::vector<cell>::const_iterator;
    using reference = std::vector<cell>::reference;
    using const_reference = std::vector<cell>::const_reference;

    cell_grid(std::size_t i_width, std::size_t i_height);

    template<class Generator>
    void generate(const Generator& i_generator)
    {
      for (const auto [x, y] : i_generator.gen())
      {
        (*this)(x, y) = cell(cell_value::BOMB);
      }

      mswpr::place_values_around_mines(*this, d_width, d_height);
      // if (PRINT_FIELD)
      //   mswpr::debug::display_grid_to_stream(std::cout, d_grid, d_width, d_height);
    }

    reference operator()(std::size_t i_x, std::size_t i_y);
    const_reference operator()(std::size_t i_x, std::size_t i_y) const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    void reset();

  private:
    std::vector<cell> d_cells;
    std::size_t d_width;
    std::size_t d_height;
  };
}  // namespace mswpr

#endif  // MSWPR_CELL_GRID_HPP
