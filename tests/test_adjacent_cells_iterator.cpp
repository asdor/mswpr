
#include "core/adjacent_cells_iterator.hpp"
#include "core/cell.hpp"
#include "core/minefield.hpp"

#include <vector>

#include <gtest/gtest.h>

namespace
{
  struct TestData
  {
    size_t width;
    size_t height;
    mswpr::cell_coord coord;
    std::vector<mswpr::cell_coord> expected_coords;
  };

  class FetchAdjacentCellsTest : public testing::TestWithParam<TestData>
  {
  };

  const TestData LeftTop = { .width = 5,
                             .height = 5,
                             .coord = { 0, 0 },
                             .expected_coords = { { 1, 0 }, { 0, 1 }, { 1, 1 } } };
  const TestData RightTop = { .width = 5,
                              .height = 5,
                              .coord = { 4, 0 },
                              .expected_coords = { { 3, 0 }, { 3, 1 }, { 4, 1 } } };
  const TestData LeftBottom = { .width = 5,
                                .height = 5,
                                .coord = { 0, 4 },
                                .expected_coords = { { 0, 3 }, { 1, 3 }, { 1, 4 } } };
  const TestData RightBottom = { .width = 5,
                                 .height = 5,
                                 .coord = { 4, 4 },
                                 .expected_coords = { { 3, 3 }, { 4, 3 }, { 3, 4 } } };
  const TestData Top = { .width = 5,
                         .height = 5,
                         .coord = { 2, 0 },
                         .expected_coords = { { 1, 0 }, { 3, 0 }, { 1, 1 }, { 2, 1 }, { 3, 1 } } };
  const TestData Right = { .width = 5,
                           .height = 5,
                           .coord = { 4, 2 },
                           .expected_coords = { { 3, 1 }, { 4, 1 }, { 3, 2 }, { 3, 3 }, { 4, 3 } } };
  const TestData Bottom = { .width = 5,
                            .height = 5,
                            .coord = { 2, 4 },
                            .expected_coords = { { 1, 3 }, { 2, 3 }, { 3, 3 }, { 1, 4 }, { 3, 4 } } };
  const TestData Left = { .width = 5,
                          .height = 5,
                          .coord = { 0, 2 },
                          .expected_coords = { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 0, 3 }, { 1, 3 } } };
  const TestData Central = {
    .width = 5,
    .height = 5,
    .coord = { 2, 2 },
    .expected_coords = { { 1, 1 }, { 2, 1 }, { 3, 1 }, { 1, 2 }, { 3, 2 }, { 1, 3 }, { 2, 3 }, { 3, 3 } }
  };
  const TestData SlimHorizontalField = { .width = 3,
                                         .height = 1,
                                         .coord = { 1, 0 },
                                         .expected_coords = { { 0, 0 }, { 2, 0 } } };
  const TestData SlimVerticalField = { .width = 1,
                                       .height = 3,
                                       .coord = { 0, 1 },
                                       .expected_coords = { { 0, 0 }, { 0, 2 } } };
  const TestData PointLikeField = { .width = 1, .height = 1, .coord = { 0, 0 }, .expected_coords = {} };
}

TEST_P(FetchAdjacentCellsTest, AdjacentCells)
{
  static constexpr size_t mines_cnt = 0;

  const auto& params = GetParam();
  mswpr::minefield field(params.width, params.height, mines_cnt);

  const auto adjacent_coords = mswpr::fetch_adjacent_cells(params.coord, params.width, params.height);
  EXPECT_EQ(adjacent_coords, params.expected_coords);
}

INSTANTIATE_TEST_SUITE_P(AdjacentCellsIterator,
                         FetchAdjacentCellsTest,
                         testing::ValuesIn({ LeftTop,
                                             RightTop,
                                             LeftBottom,
                                             RightBottom,
                                             Top,
                                             Right,
                                             Bottom,
                                             Left,
                                             Central,
                                             SlimHorizontalField,
                                             SlimVerticalField,
                                             PointLikeField }));
