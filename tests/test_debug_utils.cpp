#include "core/debug_utils.hpp"
#include "core/minefield.hpp"

#include <sstream>

#include <gtest/gtest.h>

TEST(DebugUtils, PrintGrid)
{
  static constexpr size_t width = 4;
  static constexpr size_t height = 3;
  const std::vector<size_t> mine_indices = { 6, 8 };
  mswpr::minefield field(mine_indices, width, height);

  std::ostringstream oss;
  mswpr::debug::display_grid_to_stream(oss, field.get_grid(), width, height);
  const std::string expected = " 111\n"
                               "12*1\n"
                               "*211\n";

  EXPECT_EQ(oss.str(), expected);
}
