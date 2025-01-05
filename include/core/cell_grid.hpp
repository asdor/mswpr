#ifndef MSWPR_CELL_GRID_HPP
#define MSWPR_CELL_GRID_HPP

#include "core/cell.hpp"

#include <cstddef>
#include <vector>

namespace mswpr
{
  class cell_grid
  {
  public:
    cell_grid(std::size_t i_width, std::size_t i_height);
    cell& operator[](std::size_t i_index);
    const cell& operator[](std::size_t i_index) const;

    void reset();

  private:
    std::vector<cell> d_cells;
    std::size_t d_width;
    std::size_t d_height;
  };
}  // namespace mswpr

#endif  // MSWPR_CELL_GRID_HPP
