#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <random>

#include "core/minefield.hpp"
#include "core/types.hpp"

namespace
{
  constexpr bool PRINT_FIELD = false;
}  // namespace

namespace mswpr
{
  minefield::minefield(size_t width, size_t height, size_t bombs_cnt) :
    d_width(width),
    d_height(height),
    d_bombs_cnt(bombs_cnt),
    d_unopened_cnt(d_width * d_height),
    d_grid(d_width, d_height)
  {
  }

  minefield::minefield(const std::vector<size_t>& mines_ind, size_t width, size_t height, size_t bombs_cnt) :
    minefield(width, height, bombs_cnt)
  {
    for (size_t mine_ind : mines_ind)
    {
      const size_t x_c = mine_ind % d_width;
      const size_t y_c = mine_ind / d_width;
      d_grid(x_c, y_c).value = cell_value::BOMB;
    }

    place_values_around_mines();
  }

  void minefield::place_values_around_mines()
  {
    for (size_t y = 0; y < d_height; ++y)
    {
      for (size_t x = 0; x < d_width; ++x)
      {
        if (d_grid(x, y).value == cell_value::BOMB)
          continue;

        const auto neighbours = get_neighbours({ x, y });
        const size_t cnt = std::count_if(neighbours.begin(), neighbours.end(), [&grid = d_grid](cell_coord i_v) {
          return grid(i_v.x, i_v.y).is_bomb();
        });
        d_grid(x, y).value = to_enum<cell_value>(cnt);
      }
    }
  }

  void print_field_to_cout(const cell_grid& i_grid, size_t i_width, size_t i_height)
  {
    if (!PRINT_FIELD)
      return;

    for (size_t y = 0; y < i_height; ++y)
    {
      for (size_t x = 0; x < i_width; ++x)
      {
        auto cell = i_grid(x, y).value;
        char cell_name = ' ';
        if (cell == mswpr::cell_value::EMPTY)
        {
          cell_name = ' ';
        }
        else if (cell == mswpr::cell_value::BOMB)
        {
          cell_name = '*';
        }
        else
        {
          cell_name = '1' + (enum_to<char>(cell) - enum_to<char>(mswpr::cell_value::ONE));
        }

        std::cout << cell_name;
      }
      std::cout << '\n';
    }
  }

  std::vector<size_t> minefield::get_mines_candidates(size_t x, size_t y) const
  {
    const bool is_generate_glade = (d_width > 3) && (d_height > 3) && (d_width * d_height - 9 >= d_bombs_cnt);
    const auto cur_index = y * d_width + x;

    std::vector<size_t> glade = { cur_index };
    if (is_generate_glade)
    {
      const auto neighbours_vec = get_neighbours({ x, y });
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

  void minefield::generate(size_t x, size_t y)
  {
    auto coords = get_mines_candidates(x, y);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(coords.begin(), coords.end(), g);

    for (size_t i = 0; i < d_bombs_cnt; ++i)
    {
      const size_t x_c = coords[i] % d_width;
      const size_t y_c = coords[i] / d_width;
      d_grid(x_c, y_c).value = cell_value::BOMB;
    }

    place_values_around_mines();
    print_field_to_cout(d_grid, d_width, d_height);
  }

  void minefield::reset()
  {
    d_grid.reset();

    d_unopened_cnt = d_width * d_height;
  }

  size_t minefield::get_bomb_cnt() const
  {
    return d_bombs_cnt;
  }

  cell_state minefield::get_cell_state(size_t x, size_t y) const
  {
    const auto cell = d_grid(x, y);
    return cell.state;
  }

  cell_value minefield::get_cell_value(size_t x, size_t y) const
  {
    const auto cell = d_grid(x, y);
    return cell.value;
  }

  bool minefield::is_deminied() const
  {
    return d_unopened_cnt == d_bombs_cnt;
  }

  bool minefield::is_bomb(size_t x, size_t y) const
  {
    const auto cell = d_grid(x, y);
    return cell.is_bomb();
  }

  bool minefield::is_not_flagged_bomb(size_t x, size_t y) const
  {
    const auto cell = d_grid(x, y);
    return cell.is_not_flagged_bomb();
  }

  int minefield::get_value(size_t x, size_t y) const
  {
    const auto cell = d_grid(x, y);
    return !cell.is_bomb() ? enum_to<int>(cell.value) : -1;
  }

  bool minefield::is_opened(size_t x, size_t y) const
  {
    const auto cell = d_grid(x, y);
    return cell.is_opened();
  }

  bool minefield::is_closed(size_t x, size_t y) const
  {
    const auto cell = d_grid(x, y);
    return cell.is_closed();
  }

  bool minefield::is_flagged(size_t x, size_t y) const
  {
    const auto cell = d_grid(x, y);
    return cell.is_flagged();
  }

  bool minefield::is_detonated(size_t x, size_t y) const
  {
    const auto cell = d_grid(x, y);
    return cell.is_detonated();
  }

  void minefield::open_cell(cell& i_cell)
  {
    i_cell.state = cell_state::OPENED;
    --d_unopened_cnt;
  }

  void minefield::open_cell(size_t x, size_t y)
  {
    auto& cell = d_grid(x, y);
    open_cell(cell);
  }

  void minefield::set_flag(size_t x, size_t y)
  {
    auto& cell = d_grid(x, y);
    if (cell.is_flagged())
    {
      cell.state = cell_state::CLOSED;
    }
    else if (cell.is_closed())
    {
      cell.state = cell_state::FLAGGED;
    }
  }

  void minefield::detonate_bomb(size_t x, size_t y)
  {
    auto& cell = d_grid(x, y);
    if (!cell.is_bomb())
      return;

    // is_detonated?
    cell.state = cell_state::DETONATED;
  }

  open_cell_result minefield::reveal_closed(size_t base_x, size_t base_y)
  {
    auto& base_cell = d_grid(base_x, base_y);
    if (!base_cell.is_closed())
    {
      return open_cell_result::OPENED;
    }

    if (base_cell.is_bomb())
    {
      detonate_bomb(base_x, base_y);
      return open_cell_result::DETONATED;
    }

    if (!base_cell.is_bomb() && !base_cell.is_empty())
    {
      open_cell(base_cell);
      return open_cell_result::OPENED;
    }

    std::queue<cell_coord> cells;
    cells.emplace(base_x, base_y);
    std::vector<bool> visited(d_width * d_height, false);

    while (!cells.empty())
    {
      auto cell = cells.front();
      cells.pop();
      auto& cur_cell = d_grid(cell.x, cell.y);
      if (visited[cell.y * d_width + cell.x])
        continue;

      open_cell(cur_cell);
      visited[cell.y * d_width + cell.x] = true;

      for (auto [x, y] : get_neighbours(cell))
      {
        auto& elem = d_grid(x, y);
        if (!visited[y * d_width + x] && (elem.is_empty()) && !elem.is_flagged())
        {
          cells.emplace(x, y);
        }
        else if (elem.is_closed() && !elem.is_flagged() && !elem.is_bomb())
        {
          open_cell(elem);
          visited[y * d_width + x] = true;
        }
      }
    }

    return open_cell_result::OPENED;
  }

  std::vector<cell_coord> minefield::get_neighbours(cell_coord coord) const
  {
    std::vector<cell_coord> neighbours;
    const int x_i = static_cast<int>(coord.x);
    const int y_i = static_cast<int>(coord.y);

    for (size_t i = 0; i < neighbours_x_ind.size(); ++i)
    {
      const int x_ind = neighbours_x_ind[i];
      const int y_ind = neighbours_y_ind[i];
      if (x_i < -x_ind || y_i < -y_ind)
        continue;

      const size_t x = static_cast<size_t>(x_i + x_ind);
      const size_t y = static_cast<size_t>(y_i + y_ind);
      if (x < d_width && y < d_height)
        neighbours.emplace_back(x, y);
    }

    return neighbours;
  }

  void minefield::flag_bombs()
  {
    std::vector<cell_coord> flags;

    for (size_t y = 0; y < d_height; ++y)
    {
      for (size_t x = 0; x < d_width; ++x)
      {
        if (d_grid(x, y).is_bomb())
          flags.emplace_back(x, y);
      }
    }

    for (auto [x, y] : flags)
    {
      if (!is_flagged(x, y))
      {
        set_flag(x, y);
      }
    }
  }

  void minefield::reveal_bombs()
  {
    for (size_t y = 0; y < d_height; ++y)
    {
      for (size_t x = 0; x < d_width; ++x)
      {
        auto& cell_el = d_grid(x, y);
        if (cell_el.is_bomb() && !cell_el.is_flagged() && !cell_el.is_detonated())
        {
          cell_el.state = cell_state::NOT_FLAGGED_BOMB;
        }
      }
    }
  }

}  // namespace mswpr
