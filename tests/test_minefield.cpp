#include "core/adjacent_cells_iterator.hpp"
#include "core/minefield.hpp"

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
    const int signed_width = static_cast<int>(width);
    const int signed_height = static_cast<int>(height);

    if (field.is_bomb(x, y))
      return -1;

    int count = 0;
    for (size_t i = 0; i < mswpr::neighbours_x_ind.size(); ++i)
    {
      const int i_x = static_cast<int>(x) + mswpr::neighbours_x_ind[i];
      const int i_y = static_cast<int>(y) + mswpr::neighbours_y_ind[i];
      if (i_x >= 0 && i_x < signed_width && i_y >= 0 && i_y < signed_height && field.is_bomb(i_x, i_y))
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

  // p = bomb_cnt / (width * height) - the probability that cell contains a bomb
  // 1 - p - the probability that cell doesn't contains a bomb
  // (1 - p) ^ K - the probability that cell doesn't contain a bomb in K attempts
  // P(A) = 1 - (1 - p)^ K - the probability that cell contains a bomb in K attempts
  // If the probability P(A) of opening a bomb cell with the first click tends to 1.0
  // then the test will fail if generation allows the first clicked cell to be a mine.
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

TEST(Minefield, IsDeminied)
{
  const std::vector<size_t> mines_ind = { 0 };
  const size_t width = 5;
  const size_t height = 5;
  const size_t bomb_cnt = mines_ind.size();
  mswpr::minefield field(mines_ind, width, height, bomb_cnt);

  const size_t x = 2;
  const size_t y = 2;

  EXPECT_FALSE(field.is_deminied());
  field.reveal_closed(x, y);
  EXPECT_TRUE(field.is_deminied());

  field.reset();
  EXPECT_FALSE(field.is_deminied());
}

TEST(Minefield, EmptyGladeAroundFirstCell)
{
  const size_t width = 5;
  const size_t height = 5;
  const size_t bomb_cnt = 16;
  mswpr::minefield field(width, height, bomb_cnt);

  // N = width * height - cells amount
  // K = bomb_cnt
  // C_N^K - amount of all possible field states with N cells and K bombs
  // 1 - only one state for field 5*5 with 16 bombs where central cell and its neighbors don't contain a bomb
  // 1 / C_N^K ~ 4.9 * 10^(-7) - probability of this state to be observed
  // the test will fail if generation allows the first clicked cell to have neighbors mines
  const size_t attempts = 100;
  for (size_t i = 0; i < attempts; ++i)
  {
    field.reset();
    field.generate(2, 2);
    EXPECT_EQ(field.get_value(2, 2), 0);
  }
}
