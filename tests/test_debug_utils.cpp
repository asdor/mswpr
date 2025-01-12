#include "core/debug_utils.hpp"
#include "core/minefield.hpp"
#include "test_utils/mocked_mines_generator.hpp"

#include <sstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(DebugUtils, PrintGrid)
{
  static constexpr size_t width = 4;
  static constexpr size_t height = 3;
  const mswpr::unit_tests::MockedGenerator mocked_generator({ { 2, 1 }, { 0, 2 } });

  mswpr::minefield field(width, height, mocked_generator.get_mines_cnt());
  field.generate(mocked_generator);

  std::ostringstream oss;
  mswpr::debug::display_grid_to_stream(oss, field.get_grid(), width, height);
  const std::string expected = " 111\n"
                               "12*1\n"
                               "*211\n";

  EXPECT_EQ(oss.str(), expected);
}
