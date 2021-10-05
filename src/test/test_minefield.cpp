#include "minefield.hpp"

#include <gtest/gtest.h>

using namespace mswpr;

TEST(Minefield, MainConstructor)
{
  const size_t width = 3;
  const size_t height = 3;
  const size_t bomb_cnt = 1;
  mswpr::minefield field(width, height, bomb_cnt);

  for (size_t x = 0; x < width; ++x)
  {
    for (size_t y = 0; y < height; ++y)
    {
      ASSERT_TRUE(field.is_closed(x, y));
      ASSERT_EQ(field.get_value(x, y), 0);
    }
  }
}

TEST(Minefield, ConstructorWithMines_BombWasPlaced)
{
  const std::vector<size_t> mines_ind = { 3 };
  const size_t width = 2;
  const size_t height = 2;
  const size_t bomb_cnt = mines_ind.size();
  mswpr::minefield field(mines_ind, width, height, bomb_cnt);

  ASSERT_FALSE(field.is_bomb(0, 0));
  ASSERT_EQ(field.get_value(0, 0), 1);

  ASSERT_FALSE(field.is_bomb(0, 1));
  ASSERT_EQ(field.get_value(0, 1), 1);

  ASSERT_FALSE(field.is_bomb(1, 0));
  ASSERT_EQ(field.get_value(1, 0), 1);

  ASSERT_TRUE(field.is_bomb(1, 1));
  ASSERT_EQ(field.get_value(1, 1), -1);
}

TEST(Minefield, ConstructorWithMines_AllFieldsAreClosed)
{
  const std::vector<size_t> mines_ind = { 6, 32, 50 };
  const size_t width = 8;
  const size_t height = 8;
  const size_t bomb_cnt = mines_ind.size();
  mswpr::minefield field(mines_ind, width, height, bomb_cnt);

  for (size_t x = 0; x < width; ++x)
  {
    for (size_t y = 0; y < height; ++y)
    {
      ASSERT_TRUE(field.is_closed(x, y));
    }
  }
}

TEST(Minefield, IsClosed)
{
  const size_t width = 1;
  const size_t height = 1;
  const size_t bomb_cnt = 1;
  mswpr::minefield field(width, height, bomb_cnt);

  ASSERT_TRUE(field.is_closed(0, 0));
  ASSERT_FALSE(field.is_opened(0, 0));
  ASSERT_FALSE(field.is_flagged(0, 0));
  ASSERT_FALSE(field.is_detonated(0, 0));
}

// TEST(Minefield, IsOpened)
//{
//  const size_t width = 1;
//  const size_t height = 1;
//  const size_t bomb_cnt = 1;
//  mswpr::minefield field(width, height, bomb_cnt);
//
//  field.open_cell(0, 0);
//
//  ASSERT_FALSE(field.is_closed(0, 0));
//  ASSERT_TRUE(field.is_opened(0, 0));
//  ASSERT_FALSE(field.is_flagged(0, 0));
//  ASSERT_FALSE(field.is_detonated(0, 0));
//}

TEST(Minefield, IsFlagged)
{
  const size_t width = 1;
  const size_t height = 1;
  const size_t bomb_cnt = 1;
  mswpr::minefield field(width, height, bomb_cnt);

  field.set_flag(0, 0);

  ASSERT_FALSE(field.is_closed(0, 0));
  ASSERT_FALSE(field.is_opened(0, 0));
  ASSERT_TRUE(field.is_flagged(0, 0));
  ASSERT_FALSE(field.is_detonated(0, 0));
}

TEST(Minefield, IsDetonated)
{
  const std::vector<size_t> mines_ind = { 0 };
  const size_t width = 1;
  const size_t height = 1;
  const size_t bomb_cnt = mines_ind.size();
  mswpr::minefield field(mines_ind, width, height, bomb_cnt);

  field.detonate_bomb(0, 0);

  ASSERT_FALSE(field.is_closed(0, 0));
  ASSERT_FALSE(field.is_opened(0, 0));
  ASSERT_FALSE(field.is_flagged(0, 0));
  ASSERT_TRUE(field.is_detonated(0, 0));
}

namespace
{
  int get_bombs_count(const mswpr::minefield& field, size_t width, size_t height, size_t x, size_t y)
  {
    const int width_i = static_cast<int>(width);
    const int height_i = static_cast<int>(height);

    if (field.is_bomb(x, y))
      return -1;

    int count = 0;
    for (size_t i = 0; i < minefield::neighbours_x_ind.size(); ++i)
    {
      const int i_x = static_cast<int>(x) + minefield::neighbours_x_ind[i];
      const int i_y = static_cast<int>(y) + minefield::neighbours_y_ind[i];
      if (i_x >= 0 && i_x < width && i_y >= 0 && i_y < height && field.is_bomb(i_x, i_y))
        ++count;
    }

    return count;
  }
}

TEST(Minefield, MinesCountAroundCell)
{
  const std::vector<size_t> mines_ind = { 3, 4, 5, 6, 20, 29, 36, 37, 46, 57 };
  const size_t width = 8;
  const size_t height = 8;
  const size_t bomb_cnt = mines_ind.size();
  mswpr::minefield field(mines_ind, width, height, bomb_cnt);

  for (size_t x = 0; x < width; ++x)
  {
    for (size_t y = 0; y < height; ++y)
    {
      ASSERT_EQ(field.get_value(x, y), get_bombs_count(field, width, height, x, y));
    }
  }
}

TEST(Minefield, NotBombCellAfterGeneration)
{
  const size_t width = 5;
  const size_t height = 5;
  const size_t bomb_cnt = 24;
  mswpr::minefield field(width, height, bomb_cnt);

  const size_t attempts = 100;
  for (size_t i = 0; i < attempts; ++i)
  {
    field.reset();
    field.generate(0, 0);
    EXPECT_FALSE(field.is_bomb(0, 0));
  }
}

TEST(Minefield, Reset)
{
  const size_t width = 5;
  const size_t height = 5;
  const size_t bomb_cnt = 1;
  mswpr::minefield field(width, height, bomb_cnt);

  const size_t x = 0;
  const size_t y = 0;

  field.set_flag(x, y);
  EXPECT_TRUE(field.is_flagged(x, y));

  field.reset();
  EXPECT_FALSE(field.is_flagged(x, y));
}
