#ifndef MSWPR_MINES_GENERATOR_HPP
#define MSWPR_MINES_GENERATOR_HPP

#include "core/cell_grid.hpp"

namespace mswpr
{
  void place_values_around_mines(mswpr::cell_grid& io_grid, std::size_t i_width, std::size_t i_height);
}

#endif  // MSWPR_MINES_GENERATOR_HPP
