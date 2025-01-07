
#include "core/adjacent_cells_iterator.hpp"
#include "core/cell.hpp"
#include "core/minefield.hpp"

#include <vector>

#include <gtest/gtest.h>

TEST(MinefieldRevealCells, GetNeighbours_LeftTop)
{
  mswpr::minefield field(5, 5, 0);

  mswpr::cell_coord coord{ 0, 0 };
  const auto neighbours = mswpr::fetch_adjacent_cells(coord, 5, 5);

  const std::vector<mswpr::cell_coord> cells = { { 1, 0 }, { 0, 1 }, { 1, 1 } };
  EXPECT_EQ(neighbours, cells);
}

TEST(MinefieldRevealCells, GetNeighbours_RightTop)
{
  mswpr::minefield field(5, 5, 0);

  mswpr::cell_coord coord{ 4, 0 };
  const auto neighbours = mswpr::fetch_adjacent_cells(coord, 5, 5);

  const std::vector<mswpr::cell_coord> cells = { { 3, 0 }, { 3, 1 }, { 4, 1 } };
  EXPECT_EQ(neighbours, cells);
}

TEST(MinefieldRevealCells, GetNeighbours_LeftBottom)
{
  mswpr::minefield field(5, 5, 0);

  mswpr::cell_coord coord{ 0, 4 };
  const auto neighbours = mswpr::fetch_adjacent_cells(coord, 5, 5);

  const std::vector<mswpr::cell_coord> cells = { { 0, 3 }, { 1, 3 }, { 1, 4 } };
  EXPECT_EQ(neighbours, cells);
}

TEST(MinefieldRevealCells, GetNeighbours_RightBottom)
{
  mswpr::minefield field(5, 5, 0);

  mswpr::cell_coord coord{ 4, 4 };
  const auto neighbours = mswpr::fetch_adjacent_cells(coord, 5, 5);

  const std::vector<mswpr::cell_coord> cells = { { 3, 3 }, { 4, 3 }, { 3, 4 } };
  EXPECT_EQ(neighbours, cells);
}

TEST(MinefieldRevealCells, GetNeighbours_Top)
{
  mswpr::minefield field(5, 5, 0);

  mswpr::cell_coord coord{ 2, 0 };
  const auto neighbours = mswpr::fetch_adjacent_cells(coord, 5, 5);

  const std::vector<mswpr::cell_coord> cells = { { 1, 0 }, { 3, 0 }, { 1, 1 }, { 2, 1 }, { 3, 1 } };
  EXPECT_EQ(neighbours, cells);
}

TEST(MinefieldRevealCells, GetNeighbours_Right)
{
  mswpr::minefield field(5, 5, 0);

  mswpr::cell_coord coord{ 4, 2 };
  const auto neighbours = mswpr::fetch_adjacent_cells(coord, 5, 5);

  const std::vector<mswpr::cell_coord> cells = { { 3, 1 }, { 4, 1 }, { 3, 2 }, { 3, 3 }, { 4, 3 } };
  EXPECT_EQ(neighbours, cells);
}

TEST(MinefieldRevealCells, GetNeighbours_Bottom)
{
  mswpr::minefield field(5, 5, 0);

  mswpr::cell_coord coord{ 2, 4 };
  const auto neighbours = mswpr::fetch_adjacent_cells(coord, 5, 5);

  const std::vector<mswpr::cell_coord> cells = { { 1, 3 }, { 2, 3 }, { 3, 3 }, { 1, 4 }, { 3, 4 } };
  EXPECT_EQ(neighbours, cells);
}

TEST(MinefieldRevealCells, GetNeighbours_Left)
{
  mswpr::minefield field(5, 5, 0);

  mswpr::cell_coord coord{ 0, 2 };
  const auto neighbours = mswpr::fetch_adjacent_cells(coord, 5, 5);

  const std::vector<mswpr::cell_coord> cells = { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 0, 3 }, { 1, 3 } };
  EXPECT_EQ(neighbours, cells);
}

TEST(MinefieldRevealCells, GetNeighbours_Central)
{
  mswpr::minefield field(5, 5, 0);

  mswpr::cell_coord coord{ 2, 2 };
  const auto neighbours = mswpr::fetch_adjacent_cells(coord, 5, 5);

  const std::vector<mswpr::cell_coord> cells = { { 1, 1 }, { 2, 1 }, { 3, 1 }, { 1, 2 },
                                                 { 3, 2 }, { 1, 3 }, { 2, 3 }, { 3, 3 } };
  EXPECT_EQ(neighbours, cells);
}
