#include "core/debug_utils.hpp"
#include "core/minefield.hpp"
#include "test_utils/mocked_mines_generator.hpp"

#include <sstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(DebugUtils, PrintGrid)
{
  static constexpr size_t WIDTH = 4;
  static constexpr size_t HEIGHT = 3;
  const mswpr::unit_tests::mocked_generator mocked_generator({ { 2, 1 }, { 0, 2 } });

  mswpr::minefield field(WIDTH, HEIGHT, mocked_generator.get_mines_cnt());
  field.generate(mocked_generator);

  std::ostringstream oss;
  mswpr::debug::display_grid_to_stream(oss, field.get_grid(), WIDTH, HEIGHT);
  const std::string expected = " 111\n"
                               "12*1\n"
                               "*211\n";

  EXPECT_EQ(oss.str(), expected);
}
