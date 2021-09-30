#include "minefield.hpp"

#include <gtest/gtest.h>

TEST(Minefield, OpenCell)
{
  mswpr::minefield field(3, 3, 1);

  ASSERT_TRUE(field.open_cell(0, 0));
}
