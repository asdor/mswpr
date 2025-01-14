#include "test_utils/mocked_mines_generator.hpp"

mswpr::unit_tests::mocked_generator::mocked_generator(const std::vector<mswpr::cell_coord>& i_mines) : d_mines(i_mines)
{
}

std::vector<mswpr::cell_coord> mswpr::unit_tests::mocked_generator::operator()() const
{
  return d_mines;
}

size_t mswpr::unit_tests::mocked_generator::get_mines_cnt() const
{
  return d_mines.size();
}
