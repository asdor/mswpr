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
    cell_grid(std::size_t i_count, cell i_value);
    cell& operator[](std::size_t i_index);
    const cell& operator[](std::size_t i_index) const;

    void reset();

  private:
    std::vector<cell> field_;
  };
}  // namespace mswpr

#endif  // MSWPR_CELL_GRID_HPP
