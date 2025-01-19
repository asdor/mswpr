#ifndef MSWPR_MOCKED_MINES_GENERATOR_HPP
#define MSWPR_MOCKED_MINES_GENERATOR_HPP

#include "core/cell.hpp"

#include <vector>

namespace mswpr::unit_tests
{
  class mocked_generator
  {
  public:
    mocked_generator(const std::vector<mswpr::cell_coord>& i_mines);
    std::vector<mswpr::cell_coord> operator()() const;

    size_t get_mines_cnt() const;

  private:
    std::vector<mswpr::cell_coord> d_mines;
  };
}  // namespace mswpr::unit_tests

#endif  // MSWPR_MOCKED_MINES_GENERATOR_HPP
