
#include "core/adjacent_cells_iterator.hpp"
#include "core/cell.hpp"
#include "core/minefield.hpp"

#include <vector>

#include <gtest/gtest.h>

namespace
{
  struct test_data
  {
    size_t width;
    size_t height;
    mswpr::cell_coord coord;
    std::vector<mswpr::cell_coord> expected_coords;
  };

  class fetch_adjacent_cells_test : public testing::TestWithParam<test_data>
  {
  };

  std::vector<test_data> fetch_adjacent_cells_test_suite()
  {
    static const test_data LEFT_TOP = {
      .width = 5, .height = 5, .coord = { 0, 0 }, .expected_coords = { { 1, 0 }, { 0, 1 }, { 1, 1 } }
    };
    static const test_data RIGHT_TOP = {
      .width = 5, .height = 5, .coord = { 4, 0 }, .expected_coords = { { 3, 0 }, { 3, 1 }, { 4, 1 } }
    };
    static const test_data LEFT_BOTTOM = {
      .width = 5, .height = 5, .coord = { 0, 4 }, .expected_coords = { { 0, 3 }, { 1, 3 }, { 1, 4 } }
    };
    static const test_data RIGHT_BOTTOM = {
      .width = 5, .height = 5, .coord = { 4, 4 }, .expected_coords = { { 3, 3 }, { 4, 3 }, { 3, 4 } }
    };
    static const test_data TOP = { .width = 5,
                                   .height = 5,
                                   .coord = { 2, 0 },
                                   .expected_coords = { { 1, 0 }, { 3, 0 }, { 1, 1 }, { 2, 1 }, { 3, 1 } } };
    static const test_data RIGHT = { .width = 5,
                                     .height = 5,
                                     .coord = { 4, 2 },
                                     .expected_coords = { { 3, 1 }, { 4, 1 }, { 3, 2 }, { 3, 3 }, { 4, 3 } } };
    static const test_data BOTTOM = { .width = 5,
                                      .height = 5,
                                      .coord = { 2, 4 },
                                      .expected_coords = { { 1, 3 }, { 2, 3 }, { 3, 3 }, { 1, 4 }, { 3, 4 } } };
    static const test_data LEFT = { .width = 5,
                                    .height = 5,
                                    .coord = { 0, 2 },
                                    .expected_coords = { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 0, 3 }, { 1, 3 } } };
    static const test_data CENTRAL = {
      .width = 5,
      .height = 5,
      .coord = { 2, 2 },
      .expected_coords = { { 1, 1 }, { 2, 1 }, { 3, 1 }, { 1, 2 }, { 3, 2 }, { 1, 3 }, { 2, 3 }, { 3, 3 } }
    };
    static const test_data SLIM_HORIZONTAL_FIELD = {
      .width = 3, .height = 1, .coord = { 1, 0 }, .expected_coords = { { 0, 0 }, { 2, 0 } }
    };
    static const test_data SLIM_VERTICAL_FIELD = {
      .width = 1, .height = 3, .coord = { 0, 1 }, .expected_coords = { { 0, 0 }, { 0, 2 } }
    };
    static const test_data POINT_LIKE_FIELD = { .width = 1, .height = 1, .coord = { 0, 0 }, .expected_coords = {} };
    return { LEFT_TOP,
             RIGHT_TOP,
             LEFT_BOTTOM,
             RIGHT_BOTTOM,
             TOP,
             RIGHT,
             BOTTOM,
             LEFT,
             CENTRAL,
             SLIM_HORIZONTAL_FIELD,
             SLIM_VERTICAL_FIELD,
             POINT_LIKE_FIELD };
  }
}

TEST_P(fetch_adjacent_cells_test, AdjacentCells)
{
  static constexpr size_t MINES_CNT = 0;

  const auto& params = GetParam();
  const mswpr::minefield field(params.width, params.height, MINES_CNT);

  const auto adjacent_coords = mswpr::fetch_adjacent_cells(params.coord, params.width, params.height);
  EXPECT_EQ(adjacent_coords, params.expected_coords);
}

INSTANTIATE_TEST_SUITE_P(AdjacentCellsIterator,
                         fetch_adjacent_cells_test,
                         testing::ValuesIn(fetch_adjacent_cells_test_suite()));
