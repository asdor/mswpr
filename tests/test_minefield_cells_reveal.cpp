#include "core/minefield.hpp"

#include <vector>

#include <gtest/gtest.h>

using namespace mswpr;

namespace mswpr
{
  std::ostream& operator<<(std::ostream& os, const mswpr::cell_coord& coord)
  {
    return os << '(' << coord.x << ", " << coord.y << ')';
  }
}

TEST(MinefieldRevealCells, RevealCells)
{
  const std::vector<size_t> mines_ind = { 6 };
  const size_t width = 3;
  const size_t height = 3;
  mswpr::minefield field(mines_ind, width, height);

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
  const std::vector<size_t> mines_ind = { 2, 13 };
  const size_t width = 3;
  const size_t height = 5;
  mswpr::minefield field(mines_ind, width, height);

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
  const std::vector<size_t> mines_ind = { 12 };
  const size_t width = 5;
  const size_t height = 5;
  mswpr::minefield field(mines_ind, width, height);

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
  const std::vector<size_t> mines_ind = { 0, 15 };
  const size_t width = 4;
  const size_t height = 4;
  mswpr::minefield field(mines_ind, width, height);

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
