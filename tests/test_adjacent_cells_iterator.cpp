
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

  class fetch_djacent_cells_test : public testing::TestWithParam<test_data>
  {
  };

  const test_data LeftTop = { .width = 5,
                              .height = 5,
                              .coord = { 0, 0 },
                              .expected_coords = { { 1, 0 }, { 0, 1 }, { 1, 1 } } };
  const test_data RightTop = { .width = 5,
                               .height = 5,
                               .coord = { 4, 0 },
                               .expected_coords = { { 3, 0 }, { 3, 1 }, { 4, 1 } } };
  const test_data LeftBottom = { .width = 5,
                                 .height = 5,
                                 .coord = { 0, 4 },
                                 .expected_coords = { { 0, 3 }, { 1, 3 }, { 1, 4 } } };
  const test_data RightBottom = { .width = 5,
                                  .height = 5,
                                  .coord = { 4, 4 },
                                  .expected_coords = { { 3, 3 }, { 4, 3 }, { 3, 4 } } };
  const test_data Top = { .width = 5,
                          .height = 5,
                          .coord = { 2, 0 },
                          .expected_coords = { { 1, 0 }, { 3, 0 }, { 1, 1 }, { 2, 1 }, { 3, 1 } } };
  const test_data Right = { .width = 5,
                            .height = 5,
                            .coord = { 4, 2 },
                            .expected_coords = { { 3, 1 }, { 4, 1 }, { 3, 2 }, { 3, 3 }, { 4, 3 } } };
  const test_data Bottom = { .width = 5,
                             .height = 5,
                             .coord = { 2, 4 },
                             .expected_coords = { { 1, 3 }, { 2, 3 }, { 3, 3 }, { 1, 4 }, { 3, 4 } } };
  const test_data Left = { .width = 5,
                           .height = 5,
                           .coord = { 0, 2 },
                           .expected_coords = { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 0, 3 }, { 1, 3 } } };
  const test_data Central = {
    .width = 5,
    .height = 5,
    .coord = { 2, 2 },
    .expected_coords = { { 1, 1 }, { 2, 1 }, { 3, 1 }, { 1, 2 }, { 3, 2 }, { 1, 3 }, { 2, 3 }, { 3, 3 } }
  };
  const test_data SlimHorizontalField = { .width = 3,
                                          .height = 1,
                                          .coord = { 1, 0 },
                                          .expected_coords = { { 0, 0 }, { 2, 0 } } };
  const test_data SlimVerticalField = { .width = 1,
                                        .height = 3,
                                        .coord = { 0, 1 },
                                        .expected_coords = { { 0, 0 }, { 0, 2 } } };
  const test_data PointLikeField = { .width = 1, .height = 1, .coord = { 0, 0 }, .expected_coords = {} };
}

TEST_P(fetch_djacent_cells_test, AdjacentCells)
{
  static constexpr size_t mines_cnt = 0;

  const auto& params = GetParam();
  mswpr::minefield field(params.width, params.height, mines_cnt);

  const auto adjacent_coords = mswpr::fetch_adjacent_cells(params.coord, params.width, params.height);
  EXPECT_EQ(adjacent_coords, params.expected_coords);
}

INSTANTIATE_TEST_SUITE_P(AdjacentCellsIterator,
                         fetch_djacent_cells_test,
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
