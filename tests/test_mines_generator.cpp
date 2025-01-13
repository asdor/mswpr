#include "core/mines_generator.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{
  struct TestData
  {
    size_t width;
    size_t height;
    size_t mines_cnt;
    mswpr::cell_coord clicked_cell;
    size_t expected_mines_cnt;
  };

  class GladeGeneratorTest : public testing::TestWithParam<TestData>
  {
  };

  const TestData SimpleScenario{ .width = 5,
                                 .height = 5,
                                 .mines_cnt = 8,
                                 .clicked_cell = { 2, 2 },
                                 .expected_mines_cnt = 8 };
  const TestData ZeroMines{ .width = 5,
                            .height = 5,
                            .mines_cnt = 0,
                            .clicked_cell = { 0, 0 },
                            .expected_mines_cnt = 0 };
  const TestData AllCellsAreMines{ .width = 5,
                                   .height = 5,
                                   .mines_cnt = 25,
                                   .clicked_cell = { 2, 2 },
                                   .expected_mines_cnt = 24 };
  const TestData GreaterThanFieldSizeMines{ .width = 5,
                                            .height = 5,
                                            .mines_cnt = 28,
                                            .clicked_cell = { 2, 2 },
                                            .expected_mines_cnt = 24 };
  const TestData FirstClickInAngle{ .width = 5,
                                    .height = 5,
                                    .mines_cnt = 21,
                                    .clicked_cell = { 0, 0 },
                                    .expected_mines_cnt = 21 };
  const TestData SmallField{ .width = 2,
                             .height = 2,
                             .mines_cnt = 2,
                             .clicked_cell = { 0, 1 },
                             .expected_mines_cnt = 2 };
  const TestData HorizontalField{ .width = 5,
                                  .height = 3,
                                  .mines_cnt = 4,
                                  .clicked_cell = { 0, 1 },
                                  .expected_mines_cnt = 4 };
  const TestData VerticalField{ .width = 3,
                                .height = 5,
                                .mines_cnt = 4,
                                .clicked_cell = { 1, 2 },
                                .expected_mines_cnt = 4 };
}

TEST_P(GladeGeneratorTest, DifferentFieldCombinations)
{
  const auto& params = GetParam();
  const mswpr::GladeGenerator glade_generator(
    params.width, params.height, params.mines_cnt, params.clicked_cell.x, params.clicked_cell.y);

  EXPECT_THAT(glade_generator.gen(), ::testing::SizeIs(params.expected_mines_cnt));
}

INSTANTIATE_TEST_SUITE_P(MinesGenerator,
                         GladeGeneratorTest,
                         testing::ValuesIn({ SimpleScenario,
                                             ZeroMines,
                                             AllCellsAreMines,
                                             GreaterThanFieldSizeMines,
                                             FirstClickInAngle,
                                             SmallField,
                                             HorizontalField,
                                             VerticalField }));

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
  static constexpr size_t attempts = 100;
  static constexpr mswpr::cell_coord clicked_cell{ 0, 0 };
  const mswpr::GladeGenerator glade_generator(width, height, mines_cnt, clicked_cell.x, clicked_cell.y);

  for (size_t i = 0; i < attempts; ++i)
  {
    const auto mines = glade_generator.gen();
    EXPECT_THAT(mines, Not(Contains(clicked_cell)));
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
  static constexpr size_t attempts = 100;
  static constexpr mswpr::cell_coord clicked_cell{ 2, 2 };

  // clang-format off
  const std::vector<mswpr::cell_coord> adjacent_cordinates = {
     { 1, 1 }, { 2, 1 }, { 3, 1 },
     { 1, 2 }, clicked_cell, { 3, 2 },
     { 1, 3 }, { 2, 3 }, { 3, 3 }
  };
  // clang-format on
  const mswpr::GladeGenerator glade_generator(width, height, mines_cnt, clicked_cell.x, clicked_cell.y);

  for (size_t i = 0; i < attempts; ++i)
  {
    const auto mines = glade_generator.gen();
    for (auto cell : adjacent_cordinates)
      EXPECT_THAT(mines, Not(Contains(cell)));
  }
}
