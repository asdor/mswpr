#include "core/adjacent_cells_iterator.hpp"
#include "core/minefield.hpp"
#include "core/mines_generator.hpp"
#include "test_utils/mocked_mines_generator.hpp"

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
      const auto& cell = field(x, y);
      ASSERT_TRUE(cell.is_closed());
      ASSERT_EQ(field.get_value(x, y), 0);
    }
  }
}

TEST(Minefield, Getters)
{
  const size_t width = 3;
  const size_t height = 5;
  const size_t bomb_cnt = 4;
  mswpr::minefield field(width, height, bomb_cnt);

  EXPECT_EQ(field.get_width(), width);
  EXPECT_EQ(field.get_height(), height);
  EXPECT_EQ(field.get_bomb_cnt(), bomb_cnt);
}

TEST(Minefield, ConstructorWithMines_BombWasPlaced)
{
  const size_t width = 2;
  const size_t height = 2;
  const mswpr::unit_tests::MockedGenerator mocked_generator({ { 1, 1 } });

  mswpr::minefield field(width, height, mocked_generator.get_mines_cnt());
  field.generate(mocked_generator);

  ASSERT_FALSE(field(0, 0).is_bomb());
  ASSERT_EQ(field.get_value(0, 0), 1);

  ASSERT_FALSE(field(0, 1).is_bomb());
  ASSERT_EQ(field.get_value(0, 1), 1);

  ASSERT_FALSE(field(1, 0).is_bomb());
  ASSERT_EQ(field.get_value(1, 0), 1);

  ASSERT_TRUE(field(1, 1).is_bomb());
  ASSERT_EQ(field.get_value(1, 1), -1);
}

TEST(Minefield, ConstructorWithMines_AllFieldsAreClosed)
{
  const size_t width = 8;
  const size_t height = 8;
  const mswpr::unit_tests::MockedGenerator mocked_generator({ { 6, 0 }, { 0, 4 }, { 2, 6 } });

  mswpr::minefield field(width, height, mocked_generator.get_mines_cnt());
  field.generate(mocked_generator);

  for (const auto& cell : field.get_grid())
  {
    ASSERT_TRUE(cell.is_closed());
  }
}

TEST(Minefield, IsClosed)
{
  const size_t width = 1;
  const size_t height = 1;
  const size_t bomb_cnt = 1;
  mswpr::minefield field(width, height, bomb_cnt);

  const auto cell = field(0, 0);

  ASSERT_TRUE(cell.is_closed());
  ASSERT_FALSE(cell.is_opened());
  ASSERT_FALSE(cell.is_flagged());
  ASSERT_FALSE(cell.is_detonated());
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

  field.toggle_flag(0, 0);
  const auto cell = field(0, 0);

  ASSERT_FALSE(cell.is_closed());
  ASSERT_FALSE(cell.is_opened());
  ASSERT_TRUE(cell.is_flagged());
  ASSERT_FALSE(cell.is_detonated());
}

TEST(Minefield, IsDetonated)
{
  const size_t width = 1;
  const size_t height = 1;
  const mswpr::unit_tests::MockedGenerator mocked_generator({ { 0, 0 } });

  mswpr::minefield field(width, height, mocked_generator.get_mines_cnt());
  field.generate(mocked_generator);

  field.detonate_bomb(0, 0);
  const auto cell = field(0, 0);

  ASSERT_FALSE(cell.is_closed());
  ASSERT_FALSE(cell.is_opened());
  ASSERT_FALSE(cell.is_flagged());
  ASSERT_TRUE(cell.is_detonated());
}

namespace
{
  int get_bombs_count(const mswpr::minefield& field, size_t width, size_t height, size_t x, size_t y)
  {
    const int signed_width = static_cast<int>(width);
    const int signed_height = static_cast<int>(height);

    if (field(x, y).is_bomb())
      return -1;

    int count = 0;
    for (size_t i = 0; i < mswpr::neighbours_x_ind.size(); ++i)
    {
      const int i_x = static_cast<int>(x) + mswpr::neighbours_x_ind[i];
      const int i_y = static_cast<int>(y) + mswpr::neighbours_y_ind[i];
      if (i_x >= 0 && i_x < signed_width && i_y >= 0 && i_y < signed_height && field(i_x, i_y).is_bomb())
        ++count;
    }

    return count;
  }
}

TEST(Minefield, MinesCountAroundCell)
{
  const size_t width = 8;
  const size_t height = 8;
  const mswpr::unit_tests::MockedGenerator mocked_generator(
    { { 3, 0 }, { 4, 0 }, { 5, 0 }, { 6, 0 }, { 4, 2 }, { 5, 3 }, { 4, 4 }, { 5, 4 }, { 6, 5 }, { 1, 7 } });

  mswpr::minefield field(width, height, mocked_generator.get_mines_cnt());
  field.generate(mocked_generator);

  for (size_t x = 0; x < width; ++x)
  {
    for (size_t y = 0; y < height; ++y)
    {
      ASSERT_EQ(field.get_value(x, y), get_bombs_count(field, width, height, x, y));
    }
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

  const auto& cell = field(x, y);
  field.toggle_flag(x, y);
  EXPECT_TRUE(cell.is_flagged());

  field.reset();
  EXPECT_FALSE(cell.is_flagged());
}

TEST(Minefield, IsDeminied)
{
  const size_t width = 5;
  const size_t height = 5;
  const mswpr::unit_tests::MockedGenerator mocked_generator({ { 0, 0 } });

  mswpr::minefield field(width, height, mocked_generator.get_mines_cnt());
  field.generate(mocked_generator);

  const size_t x = 2;
  const size_t y = 2;

  EXPECT_FALSE(field.is_deminied());
  field.reveal_closed(x, y);
  EXPECT_TRUE(field.is_deminied());

  field.reset();
  EXPECT_FALSE(field.is_deminied());
}
