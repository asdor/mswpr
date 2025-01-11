#include "core/cell_grid.hpp"

namespace mswpr
{
  cell_grid::cell_grid(std::size_t i_width, std::size_t i_height) : d_cells(i_width * i_height), d_width(i_width)
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
}  // namespace mswpr
