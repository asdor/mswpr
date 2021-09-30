#include "minefield.hpp"

#include <gtest/gtest.h>

TEST(Minefield, DoubleOpenCell)
{
  mswpr::minefield field(10, 10, 1);

  ASSERT_TRUE(field.open_cell(1, 1));
  ASSERT_TRUE(field.open_cell(1, 9));
}
