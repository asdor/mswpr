#include "core/mines_generator.hpp"
#include "core/adjacent_cells_iterator.hpp"
#include "core/cell.hpp"

#include <algorithm>
#include <random>

mswpr::glade_generator::glade_generator(std::size_t i_width,
                                        std::size_t i_height,
                                        std::size_t i_mines_cnt,
                                        std::size_t i_glade_center_x,
                                        std::size_t i_glade_center_y) :
  d_width(i_width),
  d_height(i_height),
  d_mines_cnt(std::min(i_mines_cnt, d_width * d_height - 1)),
  d_glade_center_x(i_glade_center_x),
  d_glade_center_y(i_glade_center_y)
{
}

std::vector<size_t> mswpr::glade_generator::get_mines_candidates(size_t i_x, size_t i_y) const
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

std::vector<mswpr::cell_coord> mswpr::glade_generator::operator()() const
{
  auto coords = get_mines_candidates(d_glade_center_x, d_glade_center_y);

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(coords.begin(), coords.end(), g);

  std::vector<mswpr::cell_coord> mines;
  mines.reserve(d_mines_cnt);

  for (size_t i = 0; i < d_mines_cnt; ++i)
  {
    const size_t x = coords[i] % d_width;
    const size_t y = coords[i] / d_width;
    mines.emplace_back(x, y);
  }
  return mines;
}
