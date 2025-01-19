#include "core/cell.hpp"

#include <gtest/gtest.h>

TEST(Cell, DefaultConstructor)
{
  const mswpr::cell cell;
  EXPECT_EQ(cell.get_value(), mswpr::cell_value::EMPTY);
  EXPECT_EQ(cell.get_state(), mswpr::cell_state::CLOSED);
}

TEST(Cell, CellValueConstructor)
{
  const mswpr::cell cell(mswpr::cell_value::THREE);
  EXPECT_EQ(cell.get_value(), mswpr::cell_value::THREE);
  EXPECT_EQ(cell.get_state(), mswpr::cell_state::CLOSED);
}

TEST(Cell, CopyConstructor)
{
  mswpr::cell cell(mswpr::cell_value::TWO);
  cell.set_state(mswpr::cell_state::OPENED);

  const mswpr::cell another_cell = cell;
  EXPECT_EQ(another_cell.get_value(), mswpr::cell_value::TWO);
  EXPECT_EQ(another_cell.get_state(), mswpr::cell_state::OPENED);
}

TEST(Cell, MoveConstructor)
{
  mswpr::cell cell(mswpr::cell_value::TWO);
  cell.set_state(mswpr::cell_state::OPENED);

  const mswpr::cell another_cell(std::move(cell));  // NOLINT(performance-move-const-arg)
  EXPECT_EQ(another_cell.get_value(), mswpr::cell_value::TWO);
  EXPECT_EQ(another_cell.get_state(), mswpr::cell_state::OPENED);
}

TEST(Cell, SetValue)
{
  const mswpr::cell cell(mswpr::cell_value::TWO);
  EXPECT_EQ(cell.get_value(), mswpr::cell_value::TWO);
}

TEST(Cell, SetState)
{
  mswpr::cell cell;
  cell.set_state(mswpr::cell_state::OPENED);
  EXPECT_EQ(cell.get_state(), mswpr::cell_state::OPENED);
}

TEST(Cell, IsEmpty)
{
  const mswpr::cell cell(mswpr::cell_value::EMPTY);
  EXPECT_TRUE(cell.is_empty());
}

TEST(Cell, IsBomb)
{
  const mswpr::cell cell(mswpr::cell_value::BOMB);
  EXPECT_TRUE(cell.is_bomb());
}

TEST(Cell, IsClosed)
{
  mswpr::cell cell;
  cell.set_state(mswpr::cell_state::CLOSED);
  EXPECT_TRUE(cell.is_closed());
}

TEST(Cell, IsOpened)
{
  mswpr::cell cell;
  cell.set_state(mswpr::cell_state::OPENED);
  EXPECT_TRUE(cell.is_opened());
}

TEST(Cell, IsFlagged)
{
  mswpr::cell cell;
  cell.set_state(mswpr::cell_state::FLAGGED);
  EXPECT_TRUE(cell.is_flagged());
}

TEST(Cell, IsDetonated)
{
  mswpr::cell cell;
  cell.set_state(mswpr::cell_state::DETONATED);
  EXPECT_TRUE(cell.is_detonated());
}

TEST(Cell, IsNotFlaggedBomb)
{
  mswpr::cell cell;
  cell.set_state(mswpr::cell_state::NOT_FLAGGED_BOMB);
  EXPECT_TRUE(cell.is_not_flagged_bomb());
}
