#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <random>

#include "core/game_config.hpp"
#include "core/minefield.hpp"
#include "core/types.hpp"

namespace
{
  constexpr bool PRINT_FIELD = false;

}  // namespace

namespace mswpr
{
  minefield::minefield(size_t width, size_t height, size_t bombs_cnt) :
    width_(width),
    height_(height),
    bombs_cnt_(bombs_cnt),
    unopened_cnt_(width_ * height_),
    field_(width_, height_)
  {
  }

  minefield::minefield(const std::vector<size_t>& mines_ind, size_t width, size_t height, size_t bombs_cnt) :
    minefield(width, height, bombs_cnt)
  {
    for (size_t mine_ind : mines_ind)
      field_[mine_ind].value = cell_value::BOMB;

    place_values_around_mines();
  }

  void minefield::place_values_around_mines()
  {
    const int width_i = static_cast<int>(width_);
    const int height_i = static_cast<int>(height_);
    for (int y = 0; y < height_i; ++y)
    {
      for (int x = 0; x < width_i; ++x)
      {
        if (field_[y * width_ + x].value == cell_value::BOMB)
          continue;

        size_t cnt = 0;
        for (size_t i = 0; i < neighbours_x_ind.size(); ++i)
        {
          int i_x = x - neighbours_x_ind[i];
          int i_y = y - neighbours_y_ind[i];
          if (i_x >= 0 && i_x < width_i && i_y >= 0 && i_y < height_i && field_[i_y * width_ + i_x].is_bomb())
            ++cnt;
        }

        field_[y * width_ + x].value = to_enum<cell_value>(cnt);
      }
    }
  }

  void print_field_to_cout(const std::vector<mswpr::cell>& field, size_t width, size_t height)
  {
    if (!PRINT_FIELD)
      return;

    const int width_i = static_cast<int>(width);
    const int height_i = static_cast<int>(height);
    for (int y = 0; y < height_i; ++y)
    {
      for (int x = 0; x < width_i; ++x)
      {
        auto cell = field[y * width + x].value;
        char cell_name;
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
          cell_name = '1' + (static_cast<char>(cell) - static_cast<char>(mswpr::cell_value::ONE));
        }

        std::cout << cell_name;
      }
      std::cout << '\n';
    }
  }

  std::vector<size_t> minefield::get_mines_candidates(size_t x, size_t y) const
  {
    const bool is_generate_glade = (width_ > 3) && (height_ > 3) && (width_ * height_ - 9 >= bombs_cnt_);
    const auto cur_index = y * width_ + x;

    std::vector<size_t> glade = { cur_index };
    if (is_generate_glade)
    {
      const auto neighbours_vec = get_neighbours({ x, y });
      for (const auto cell : neighbours_vec)
      {
        glade.push_back(cell.y * width_ + cell.x);
      }
    }

    std::vector<size_t> coords;
    for (size_t i = 0; i < width_ * height_; ++i)
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

    for (size_t i = 0; i < bombs_cnt_; ++i)
    {
      field_[coords[i]].value = cell_value::BOMB;
    }

    place_values_around_mines();
    // print_field_to_cout(field_, width_, height_);
  }

  void minefield::reset()
  {
    field_.reset();

    unopened_cnt_ = width_ * height_;
  }

  size_t minefield::get_bomb_cnt() const
  {
    return bombs_cnt_;
  }

  cell_state minefield::get_cell_state(size_t x, size_t y) const
  {
    const auto cell = field_[y * width_ + x];
    return cell.state;
  }

  cell_value minefield::get_cell_value(size_t x, size_t y) const
  {
    const auto cell = field_[y * width_ + x];
    return cell.value;
  }

  bool minefield::is_deminied() const
  {
    return unopened_cnt_ == bombs_cnt_;
  }

  bool minefield::is_bomb(size_t x, size_t y) const
  {
    const auto cell = field_[y * width_ + x];
    return cell.is_bomb();
  }

  bool minefield::is_not_flagged_bomb(size_t x, size_t y) const
  {
    const auto cell = field_[y * width_ + x];
    return cell.is_not_flagged_bomb();
  }

  int minefield::get_value(size_t x, size_t y) const
  {
    const auto cell = field_[y * width_ + x];
    return !cell.is_bomb() ? static_cast<int>(cell.value) : -1;
  }

  bool minefield::is_opened(size_t x, size_t y) const
  {
    const auto cell = field_[y * width_ + x];
    return cell.is_opened();
  }

  bool minefield::is_closed(size_t x, size_t y) const
  {
    const auto cell = field_[y * width_ + x];
    return cell.is_closed();
  }

  bool minefield::is_flagged(size_t x, size_t y) const
  {
    const auto cell = field_[y * width_ + x];
    return cell.is_flagged();
  }

  bool minefield::is_detonated(size_t x, size_t y) const
  {
    const auto cell = field_[y * width_ + x];
    return cell.is_detonated();
  }

  void minefield::open_cell(cell& i_cell)
  {
    i_cell.state = cell_state::OPENED;
    --unopened_cnt_;
  }

  void minefield::open_cell(size_t x, size_t y)
  {
    auto& cell = field_[y * width_ + x];
    open_cell(cell);
  }

  void minefield::set_flag(size_t x, size_t y)
  {
    auto& cell = field_[y * width_ + x];
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
    auto& cell = field_[y * width_ + x];
    if (!cell.is_bomb())
      return;

    // is_detonated?
    cell.state = cell_state::DETONATED;
  }

  open_cell_result minefield::reveal_closed(size_t base_x, size_t base_y)
  {
    auto& base_cell = field_[base_y * width_ + base_x];
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
    std::vector<bool> visited(width_ * height_, false);

    while (!cells.empty())
    {
      auto cell = cells.front();
      cells.pop();
      auto& cur_cell = field_[cell.y * width_ + cell.x];
      if (visited[cell.y * width_ + cell.x])
        continue;

      open_cell(cur_cell);
      visited[cell.y * width_ + cell.x] = true;

      for (auto [x, y] : get_neighbours(cell))
      {
        auto& elem = field_[y * width_ + x];
        if (!visited[y * width_ + x] && (elem.is_empty()) && !elem.is_flagged())
        {
          cells.emplace(x, y);
        }
        else if (elem.is_closed() && !elem.is_flagged() && !elem.is_bomb())
        {
          open_cell(elem);
          visited[y * width_ + x] = true;
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
      if (x < width_ && y < height_)
        neighbours.emplace_back(x, y);
    }

    return neighbours;
  }

  void minefield::flag_bombs()
  {
    std::vector<cell_coord> flags;

    const int width_i = static_cast<int>(width_);
    const int height_i = static_cast<int>(height_);
    for (int y = 0; y < height_i; ++y)
    {
      for (int x = 0; x < width_i; ++x)
      {
        if (field_[y * width_ + x].is_bomb())
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
    const int width_i = static_cast<int>(width_);
    const int height_i = static_cast<int>(height_);
    for (int y = 0; y < height_i; ++y)
    {
      for (int x = 0; x < width_i; ++x)
      {
        auto& cell_el = field_[y * width_ + x];
        if (cell_el.is_bomb() && !cell_el.is_flagged() && !cell_el.is_detonated())
        {
          cell_el.state = cell_state::NOT_FLAGGED_BOMB;
        }
      }
    }
  }

}  // namespace mswpr
