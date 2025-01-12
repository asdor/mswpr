#ifndef MSWPR_MINES_GENERATOR_HPP
#define MSWPR_MINES_GENERATOR_HPP

#include "core/cell.hpp"

#include <cstddef>
#include <vector>

namespace mswpr
{
  class cell_grid;

  void place_values_around_mines(mswpr::cell_grid& io_grid, std::size_t i_width, std::size_t i_height);

  class GladeGenerator
  {
  public:
    GladeGenerator(std::size_t i_width, std::size_t i_height, std::size_t i_mines_cnt);

    std::vector<cell_coord> gen(std::size_t i_x, std::size_t i_y) const;

  private:
    std::vector<size_t> get_mines_candidates(std::size_t i_x, std::size_t i_y) const;

    std::size_t d_width;
    std::size_t d_height;
    std::size_t d_mines_cnt;
  };
}

#endif  // MSWPR_MINES_GENERATOR_HPP
