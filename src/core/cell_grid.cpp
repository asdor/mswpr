#include "core/cell_grid.hpp"

namespace
{
  constexpr mswpr::cell EMPTY_CELL{ mswpr::cell_value::EMPTY, mswpr::cell_state::CLOSED };
}  // namespace

namespace mswpr
{
  cell_grid::cell_grid(std::size_t i_width, std::size_t i_height) :
    d_cells(i_width * i_height, EMPTY_CELL),
    d_width(i_width)
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

  const std::vector<cell>::const_iterator cell_grid::begin() const
  {
    return d_cells.cbegin();
  }

  const std::vector<cell>::const_iterator cell_grid::end() const
  {
    return d_cells.cend();
  }

  void cell_grid::reset()
  {
    std::fill(d_cells.begin(), d_cells.end(), EMPTY_CELL);
  }
}  // namespace mswpr
