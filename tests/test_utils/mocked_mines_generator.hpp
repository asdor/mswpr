#ifndef MSWPR_MOCKED_MINES_GENERATOR_HPP
#define MSWPR_MOCKED_MINES_GENERATOR_HPP

#include "core/cell.hpp"

#include <vector>

namespace mswpr::unit_tests
{
  class MockedGenerator
  {
  public:
    MockedGenerator(const std::vector<mswpr::cell_coord>& i_mines);
    std::vector<mswpr::cell_coord> gen() const;

    size_t get_mines_cnt() const;

  private:
    std::vector<mswpr::cell_coord> d_mines;
  };
}

#endif  // MSWPR_MOCKED_MINES_GENERATOR_HPP
