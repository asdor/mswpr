#ifndef MSWPR_MINES_GENERATOR_HPP
#define MSWPR_MINES_GENERATOR_HPP

#include "core/cell.hpp"

#include <cstddef>
#include <vector>

namespace mswpr
{

  class GladeGenerator
  {
  public:
    GladeGenerator(std::size_t i_width,
                   std::size_t i_height,
                   std::size_t i_mines_cnt,
                   std::size_t i_glade_center_x,
                   std::size_t i_glade_center_y);

    std::vector<cell_coord> gen() const;

  private:
    std::vector<size_t> get_mines_candidates(std::size_t i_x, std::size_t i_y) const;

    std::size_t d_width;
    std::size_t d_height;
    std::size_t d_mines_cnt;
    std::size_t d_glade_center_x;
    std::size_t d_glade_center_y;
  };
}

#endif  // MSWPR_MINES_GENERATOR_HPP
