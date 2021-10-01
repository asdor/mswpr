#include "minefield.hpp"

#include <gtest/gtest.h>

TEST(MinefieldRevealCells, RevealCells)
{
  const std::vector<size_t> mines_ind = { 6 };
  const size_t width = 3;
  const size_t height = 3;
  const size_t bomb_cnt = mines_ind.size();
  mswpr::minefield field(mines_ind, width, height, bomb_cnt);

  field.reveal_closed(1, 0);

  for (size_t y = 0; y < height; ++y)
  {
    for (size_t x = 0; x < width; ++x)
    {
      if (field.is_bomb(x, y))
      {
        EXPECT_TRUE(field.is_closed(x, y)) << "at (" << x << ", " << y << ")\n";
      }
      else
      {
        EXPECT_TRUE(field.is_opened(x, y)) << "at (" << x << ", " << y << ")\n";
      }
    }
  }
}

TEST(MinefieldRevealCells, AngleComponent)
{
  const std::vector<size_t> mines_ind = { 2, 13 };
  const size_t width = 3;
  const size_t height = 5;
  const size_t bomb_cnt = mines_ind.size();
  mswpr::minefield field(mines_ind, width, height, bomb_cnt);

  field.reveal_closed(0, 2);

  for (size_t y = 0; y < height - 1; ++y)
  {
    for (size_t x = 0; x < width; ++x)
    {
      if (field.is_bomb(x, y))
      {
        EXPECT_TRUE(field.is_closed(x, y)) << "at (" << x << ", " << y << ")\n";
      }
      else
      {
        EXPECT_TRUE(field.is_opened(x, y)) << "at (" << x << ", " << y << ")\n";
      }
    }
  }

  // process last line manually
  EXPECT_TRUE(field.is_closed(0, 4));
  EXPECT_TRUE(field.is_closed(1, 4));
  EXPECT_TRUE(field.is_closed(2, 4));
}

TEST(MinefieldRevealCells, MineInTheMiddleComponent)
{
  const std::vector<size_t> mines_ind = { 12 };
  const size_t width = 5;
  const size_t height = 5;
  const size_t bomb_cnt = mines_ind.size();
  mswpr::minefield field(mines_ind, width, height, bomb_cnt);

  field.reveal_closed(0, 0);

  for (size_t y = 0; y < height; ++y)
  {
    for (size_t x = 0; x < width; ++x)
    {
      if (field.is_bomb(x, y))
      {
        EXPECT_TRUE(field.is_closed(x, y)) << "at (" << x << ", " << y << ")\n";
      }
      else
      {
        EXPECT_TRUE(field.is_opened(x, y)) << "at (" << x << ", " << y << ")\n";
      }
    }
  }
}

TEST(MinefieldRevealCells, OneComponentByDiagonal)
{
  const std::vector<size_t> mines_ind = { 0, 15 };
  const size_t width = 4;
  const size_t height = 4;
  const size_t bomb_cnt = mines_ind.size();
  mswpr::minefield field(mines_ind, width, height, bomb_cnt);

  field.reveal_closed(3, 0);

  for (size_t y = 0; y < height; ++y)
  {
    for (size_t x = 0; x < width; ++x)
    {
      if (field.is_bomb(x, y))
      {
        EXPECT_TRUE(field.is_closed(x, y)) << "at (" << x << ", " << y << ")\n";
      }
      else
      {
        EXPECT_TRUE(field.is_opened(x, y)) << "at (" << x << ", " << y << ")\n";
      }
    }
  }
}
