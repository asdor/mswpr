#include "core/mines_generator.hpp"
#include "core/adjacent_cells_iterator.hpp"
#include "core/cell.hpp"
#include "core/cell_grid.hpp"
#include "core/types.hpp"

#include <algorithm>
#include <random>

void mswpr::place_values_around_mines(mswpr::cell_grid& io_grid, std::size_t i_width, std::size_t i_height)
{
  for (size_t y = 0; y < i_height; ++y)
  {
    for (size_t x = 0; x < i_width; ++x)
    {
      auto& cell = io_grid(x, y);
      if (cell.is_bomb())
        continue;

      const auto neighbours = fetch_adjacent_cells({ x, y }, i_width, i_height);
      const size_t cnt = std::count_if(neighbours.begin(), neighbours.end(), [&io_grid](cell_coord i_cell) {
        return io_grid(i_cell.x, i_cell.y).is_bomb();
      });
      cell.set_value(to_enum<cell_value>(cnt));
    }
  }
}

mswpr::GladeGenerator::GladeGenerator(std::size_t i_width, std::size_t i_height, std::size_t i_mines_cnt) :
  d_width(i_width),
  d_height(i_height),
  d_mines_cnt(i_mines_cnt)
{
}

std::vector<size_t> mswpr::GladeGenerator::get_mines_candidates(size_t i_x, size_t i_y) const
{
  const bool is_generate_glade = (d_width > 3) && (d_height > 3) && (d_width * d_height - 9 >= d_mines_cnt);
  const auto cur_index = i_y * d_width + i_x;

  std::vector<size_t> glade = { cur_index };
  if (is_generate_glade)
  {
    const auto neighbours_vec = fetch_adjacent_cells({ i_x, i_y }, d_width, d_height);
    for (const auto cell : neighbours_vec)
    {
      glade.push_back(cell.y * d_width + cell.x);
    }
  }

  std::vector<size_t> coords;
  for (size_t i = 0; i < d_width * d_height; ++i)
  {
    if (auto it = std::find(glade.begin(), glade.end(), i); it == glade.end())
    {
      coords.push_back(i);
    }
  }

  return coords;
}

std::vector<mswpr::cell_coord> mswpr::GladeGenerator::gen(std::size_t i_x, std::size_t i_y) const
{
  auto coords = get_mines_candidates(i_x, i_y);

  std::vector<cell_coord> mines;
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(coords.begin(), coords.end(), g);

  std::vector<mswpr::cell_coord> result;
  result.reserve(d_mines_cnt);

  for (size_t i = 0; i < d_mines_cnt; ++i)
  {
    const size_t x = coords[i] % d_width;
    const size_t y = coords[i] / d_width;
    result.emplace_back(x, y);
  }
  return result;
}
