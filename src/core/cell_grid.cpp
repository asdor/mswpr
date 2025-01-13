#include "core/cell_grid.hpp"
#include "core/adjacent_cells_iterator.hpp"
#include "core/types.hpp"

#include <algorithm>

namespace mswpr
{
  cell_grid::cell_grid(std::size_t i_width, std::size_t i_height) :
    d_cells(i_width * i_height),
    d_width(i_width),
    d_height(i_height)
  {
  }

  cell_grid::reference cell_grid::operator()(std::size_t i_x, std::size_t i_y)
  {
    return d_cells[i_y * d_width + i_x];
  }

  cell_grid::const_reference cell_grid::operator()(std::size_t i_x, std::size_t i_y) const
  {
    return d_cells[i_y * d_width + i_x];
  }

  cell_grid::iterator cell_grid::begin()
  {
    return d_cells.begin();
  }

  cell_grid::iterator cell_grid::end()
  {
    return d_cells.end();
  }

  cell_grid::const_iterator cell_grid::begin() const
  {
    return d_cells.begin();
  }

  cell_grid::const_iterator cell_grid::end() const
  {
    return d_cells.end();
  }

  void cell_grid::reset()
  {
    static constexpr cell empty_cell{};
    std::fill(d_cells.begin(), d_cells.end(), empty_cell);
  }

  void mswpr::cell_grid::place_values_around_mines()
  {
    for (size_t y = 0; y < d_height; ++y)
    {
      for (size_t x = 0; x < d_width; ++x)
      {
        auto& cell = (*this)(x, y);
        if (cell.is_bomb())
          continue;

        const auto neighbours = fetch_adjacent_cells({ x, y }, d_width, d_height);
        const auto is_bomb = [width = d_width, &cells = d_cells](cell_coord i_cell) {
          return cells[i_cell.y * width + i_cell.x].is_bomb();
        };
        const size_t cnt = std::ranges::count_if(neighbours, is_bomb);
        cell = mswpr::cell(to_enum<cell_value>(cnt));
      }
    }
  }
}

// namespace mswpr
