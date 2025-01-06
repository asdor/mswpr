#ifndef MSWPR_DEBUG_UTILS_HPP
#define MSWPR_DEBUG_UTILS_HPP

#include <cstddef>
#include <ostream>

namespace mswpr
{
  class cell_grid;

  namespace debug
  {
    void display_grid_to_stream(std::ostream& io_os,
                                const mswpr::cell_grid& i_grid,
                                std::size_t i_width,
                                std::size_t i_height);
  }  // namespace debug
}

#endif  // MSWPR_DEBUG_UTILS_HPP
