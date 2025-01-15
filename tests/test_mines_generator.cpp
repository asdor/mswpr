#include "core/mines_generator.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{
  struct test_data
  {
    size_t width;
    size_t height;
    size_t mines_cnt;
    mswpr::cell_coord clicked_cell;
    size_t expected_mines_cnt;
  };

  class glade_generator_test : public testing::TestWithParam<test_data>
  {
  };

  const test_data SIMPLE_SCENARIO{ .width = 5,
                                   .height = 5,
                                   .mines_cnt = 8,
                                   .clicked_cell = { 2, 2 },
                                   .expected_mines_cnt = 8 };
  const test_data ZERO_MINES{ .width = 5,
                              .height = 5,
                              .mines_cnt = 0,
                              .clicked_cell = { 0, 0 },
                              .expected_mines_cnt = 0 };
  const test_data ALL_CELLS_ARE_MINES{ .width = 5,
                                       .height = 5,
                                       .mines_cnt = 25,
                                       .clicked_cell = { 2, 2 },
                                       .expected_mines_cnt = 24 };
  const test_data GREATER_THAN_FIELD_SIZE_MINES{ .width = 5,
                                                 .height = 5,
                                                 .mines_cnt = 28,
                                                 .clicked_cell = { 2, 2 },
                                                 .expected_mines_cnt = 24 };
  const test_data FIRST_CLICK_IN_ANGLE{ .width = 5,
                                        .height = 5,
                                        .mines_cnt = 21,
                                        .clicked_cell = { 0, 0 },
                                        .expected_mines_cnt = 21 };
  const test_data SMALL_FIELD{ .width = 2,
                               .height = 2,
                               .mines_cnt = 2,
                               .clicked_cell = { 0, 1 },
                               .expected_mines_cnt = 2 };
  const test_data HORIZONTAL_FIELD{ .width = 5,
                                    .height = 3,
                                    .mines_cnt = 4,
                                    .clicked_cell = { 0, 1 },
                                    .expected_mines_cnt = 4 };
  const test_data VERTICAL_FIELD{ .width = 3,
                                  .height = 5,
                                  .mines_cnt = 4,
                                  .clicked_cell = { 1, 2 },
                                  .expected_mines_cnt = 4 };
}

TEST_P(glade_generator_test, DifferentFieldCombinations)
{
  const auto& params = GetParam();
  const mswpr::glade_generator glade_generator(
    params.width, params.height, params.mines_cnt, params.clicked_cell.x, params.clicked_cell.y);

  EXPECT_THAT(glade_generator(), ::testing::SizeIs(params.expected_mines_cnt));
}

INSTANTIATE_TEST_SUITE_P(MinesGenerator,
                         glade_generator_test,
                         testing::ValuesIn({ SIMPLE_SCENARIO,
                                             ZERO_MINES,
                                             ALL_CELLS_ARE_MINES,
                                             GREATER_THAN_FIELD_SIZE_MINES,
                                             FIRST_CLICK_IN_ANGLE,
                                             SMALL_FIELD,
                                             HORIZONTAL_FIELD,
                                             VERTICAL_FIELD }));

TEST(MinesGenerator, NotBombCellAfterGeneration)
{
  // This test verifies that the generator ensures that there are no mines in input cell {x, y}.
  //
  // Let us define the following events:
  // A: For N attempts, at least one mine is generated in cell {x, y}.
  // B: For N attempts, no mines are generated in cell {x, y}.
  // C: A mine is generated in cell {x, y} for a single attempt.
  //
  // Probabilities:
  // P(C) = mines_cnt / (width * height)
  // P(B) = P(!C) ^ N = (1 - P(C))^N
  // P(A) = P(!B) = 1 - P(B) = 1 - (1 - P(C))^N
  //
  // For a width of 5, height of 5, and mines_cnt = 24, P(A) ≈ 1 - 10^(-140).
  // If the generation algorithm is broken, this test will fail with an extremely high likelihood.

  using ::testing::Contains;
  using ::testing::Not;

  const size_t width = 5;
  const size_t height = 5;
  const size_t mines_cnt = 24;
  static constexpr size_t ATTEMPTS = 100;
  static constexpr mswpr::cell_coord CLICKED_CELL{ 0, 0 };
  const mswpr::glade_generator glade_generator(width, height, mines_cnt, CLICKED_CELL.x, CLICKED_CELL.y);

  for (size_t i = 0; i < ATTEMPTS; ++i)
  {
    const auto mines = glade_generator();
    EXPECT_THAT(mines, Not(Contains(CLICKED_CELL)));
  }
}

TEST(MinesGenerator, EmptyGladeAroundFirstCell)
{
  // This test verifies that the generator ensures the 3x3 glade around the input cell {x, y} contains no mines.
  //
  // Let us define the following events:
  // A: At least one mine is generated in the 3x3 glade around {x, y}.
  // B: No mines are generated in the 3x3 glade around {x, y}.
  // N = width * height - the total number of cells in the field.
  //
  // Probabilities:
  // P(B) = 1 / C(N, 8)
  // P(A) = P(!B) = 1 - P(B) = 1 - 1 / C(N, 8)
  //
  // For a field with width = 5, height = 5, and mines_cnt = 16, P(A) ≈ 1- 4.9 * 10^(-7).
  // If the generation algorithm is broken, this test will fail with an extremely high likelihood.

  using ::testing::Contains;
  using ::testing::Not;

  const size_t width = 5;
  const size_t height = 5;
  const size_t mines_cnt = 16;
  static constexpr size_t ATTEMPTS = 100;
  static constexpr mswpr::cell_coord CLICKED_CELL{ 2, 2 };

  // clang-format off
  const std::vector<mswpr::cell_coord> adjacent_cordinates = {
     { 1, 1 }, { 2, 1 }, { 3, 1 },
     { 1, 2 }, CLICKED_CELL, { 3, 2 },
     { 1, 3 }, { 2, 3 }, { 3, 3 }
  };
  // clang-format on
  const mswpr::glade_generator glade_generator(width, height, mines_cnt, CLICKED_CELL.x, CLICKED_CELL.y);

  for (size_t i = 0; i < ATTEMPTS; ++i)
  {
    const auto mines = glade_generator();
    for (auto cell : adjacent_cordinates)
      EXPECT_THAT(mines, Not(Contains(cell)));
  }
}
