#ifndef MSWPR_ADJACENT_CELLS_ITERATOR_HPP
#define MSWPR_ADJACENT_CELLS_ITERATOR_HPP

#include "core/cell.hpp"

#include <array>
#include <vector>

namespace mswpr
{
  inline constexpr std::array<int, 8> neighbours_x_ind = { -1, 0, 1, -1, 1, -1, 0, 1 };
  inline constexpr std::array<int, 8> neighbours_y_ind = { -1, -1, -1, 0, 0, 1, 1, 1 };

  std::vector<mswpr::cell_coord> fetch_adjacent_cells(mswpr::cell_coord i_coord,
                                                      std::size_t i_width,
                                                      std::size_t i_height);
}

#endif  // MSWPR_ADJACENT_CELLS_ITERATOR_HPP
