#include "core/cell_grid.hpp"

namespace mswpr
{
  cell_grid::cell_grid(std::size_t i_count, cell i_value) : field_(i_count, i_value)
  {
  }

  cell& cell_grid::operator[](std::size_t i_index)
  {
    return field_[i_index];
  }

  const cell& cell_grid::operator[](std::size_t i_index) const
  {
    return field_[i_index];
  }

  void cell_grid::reset()
  {
    cell empty{ cell_value::EMPTY, cell_state::CLOSED };
    std::fill(field_.begin(), field_.end(), empty);
  }
}  // namespace mswpr
