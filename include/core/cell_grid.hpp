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
    using iterator = std::vector<cell>::iterator;
    using const_iterator = std::vector<cell>::const_iterator;
    using reference = std::vector<cell>::reference;
    using const_reference = std::vector<cell>::const_reference;

    cell_grid(std::size_t i_width, std::size_t i_height);
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
  };
}  // namespace mswpr

#endif  // MSWPR_CELL_GRID_HPP
