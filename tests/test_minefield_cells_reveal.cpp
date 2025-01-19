#include "core/minefield.hpp"
#include "test_utils/mocked_mines_generator.hpp"

#include <vector>

#include <gtest/gtest.h>

TEST(MinefieldRevealCells, RevealCells)
{
  const size_t width = 3;
  const size_t height = 3;
  const mswpr::unit_tests::mocked_generator mocked_generator({ { 0, 2 } });

  mswpr::minefield field(width, height, mocked_generator.get_mines_cnt());
  field.generate(mocked_generator);

  field.reveal_closed(1, 0);

  for (size_t y = 0; y < field.get_height(); ++y)
  {
    for (size_t x = 0; x < field.get_width(); ++x)
    {
      const auto& cell = field(x, y);
      if (cell.is_bomb())
      {
        EXPECT_TRUE(cell.is_closed()) << "at (" << x << ", " << y << ")\n";
      }
      else
      {
        EXPECT_TRUE(cell.is_opened()) << "at (" << x << ", " << y << ")\n";
      }
    }
  }
}

TEST(MinefieldRevealCells, AngleComponent)
{
  const size_t width = 3;
  const size_t height = 5;
  const mswpr::unit_tests::mocked_generator mocked_generator({ { 2, 0 }, { 1, 4 } });

  mswpr::minefield field(width, height, mocked_generator.get_mines_cnt());
  field.generate(mocked_generator);

  field.reveal_closed(0, 2);

  for (size_t y = 0; y < field.get_height() - 1; ++y)
  {
    for (size_t x = 0; x < field.get_width(); ++x)
    {
      const auto& cell = field(x, y);
      if (cell.is_bomb())
      {
        EXPECT_TRUE(cell.is_closed()) << "at (" << x << ", " << y << ")\n";
      }
      else
      {
        EXPECT_TRUE(cell.is_opened()) << "at (" << x << ", " << y << ")\n";
      }
    }
  }

  // process last line manually
  EXPECT_TRUE(field(0, 4).is_closed());
  EXPECT_TRUE(field(1, 4).is_closed());
  EXPECT_TRUE(field(2, 4).is_closed());
}

TEST(MinefieldRevealCells, MineInTheMiddleComponent)
{
  const size_t width = 5;
  const size_t height = 5;
  const mswpr::unit_tests::mocked_generator mocked_generator({ { 2, 2 } });

  mswpr::minefield field(width, height, mocked_generator.get_mines_cnt());
  field.generate(mocked_generator);

  field.reveal_closed(0, 0);

  for (size_t y = 0; y < field.get_height(); ++y)
  {
    for (size_t x = 0; x < field.get_width(); ++x)
    {
      const auto& cell = field(x, y);
      if (cell.is_bomb())
      {
        EXPECT_TRUE(cell.is_closed()) << "at (" << x << ", " << y << ")\n";
      }
      else
      {
        EXPECT_TRUE(cell.is_opened()) << "at (" << x << ", " << y << ")\n";
      }
    }
  }
}

TEST(MinefieldRevealCells, OneComponentByDiagonal)
{
  const size_t width = 4;
  const size_t height = 4;
  const mswpr::unit_tests::mocked_generator mocked_generator({ { 0, 0 }, { 3, 3 } });

  mswpr::minefield field(width, height, mocked_generator.get_mines_cnt());
  field.generate(mocked_generator);

  field.reveal_closed(3, 0);

  for (size_t y = 0; y < field.get_height(); ++y)
  {
    for (size_t x = 0; x < field.get_width(); ++x)
    {
      const auto& cell = field(x, y);
      if (cell.is_bomb())
      {
        EXPECT_TRUE(cell.is_closed()) << "at (" << x << ", " << y << ")\n";
      }
      else
      {
        EXPECT_TRUE(cell.is_opened()) << "at (" << x << ", " << y << ")\n";
      }
    }
  }
}
