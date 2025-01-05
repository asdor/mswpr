#include "core/cell_grid.hpp"

namespace
{
  constexpr mswpr::cell EMPTY_CELL{ mswpr::cell_value::EMPTY, mswpr::cell_state::CLOSED };
}  // namespace

namespace mswpr
{
  cell_grid::cell_grid(std::size_t i_width, std::size_t i_height) :
    d_cells(i_width * i_height, EMPTY_CELL),
    d_width(i_width),
    d_height(i_height)
  {
  }

  cell& cell_grid::operator()(std::size_t i_x, std::size_t i_y)
  {
    return d_cells[i_y * d_width + i_x];
  }

  const cell& cell_grid::operator()(std::size_t i_x, std::size_t i_y) const
  {
    return d_cells[i_y * d_width + i_x];
  }

  void cell_grid::reset()
  {
    std::fill(d_cells.begin(), d_cells.end(), EMPTY_CELL);
  }
}  // namespace mswpr
