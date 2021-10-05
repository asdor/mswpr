#include <algorithm>
#include <random>
#include <queue>
#include <numeric>
#include <iostream>

#include "game_config.hpp"
#include "minefield.hpp"
#include "texture_manager.hpp"
#include "types.hpp"

namespace
{
  constexpr bool PRINT_FIELD = false;

  constexpr mswpr::sprite_type to_sprite(mswpr::cell_value value)
  {
    const size_t index = mswpr::enum_to<size_t>(value);
    constexpr size_t empty_index = mswpr::enum_to<size_t>(mswpr::sprite_type::EMPTY_OPENED);
    return mswpr::to_enum<mswpr::sprite_type>(empty_index + index);
  }
}  // namespace

namespace mswpr
{
  bool cell::is_empty() const
  {
    return value == cell_value::EMPTY;
  }

  bool cell::is_bomb() const
  {
    return value == cell_value::BOMB;
  }

  bool cell::is_closed() const
  {
    return state == cell_state::CLOSED;
  }

  bool cell::is_opened() const
  {
    return state == cell_state::OPENED;
  }

  bool cell::is_flagged() const
  {
    return state == cell_state::FLAGGED;
  }

  bool cell::is_detonated() const
  {
    return state == cell_state::DETONATED;
  }

  minefield::minefield(size_t width, size_t height, size_t bombs_cnt) :
    width_(width),
    height_(height),
    bombs_cnt_(bombs_cnt),
    unopened_cnt_(width_ * height_),
    field_(width_ * height_, { cell_value::EMPTY, cell_state::CLOSED })
  {
  }

  minefield::minefield(const std::vector<size_t>& mines_ind, size_t width, size_t height, size_t bombs_cnt) :
    minefield(width, height, bombs_cnt)
  {
    for (size_t mine_ind : mines_ind)
      field_[mine_ind].value = cell_value::BOMB;

    place_values_around_mines();
  }

  void minefield::render(texture_manager& manager)
  {
    for (size_t i = 0; i < height_; ++i)
    {
      for (size_t j = 0; j < width_; ++j)
      {
        const auto item = field_[i * width_ + j];
        const SDL_Rect dst_rect = { int(j * cfg::cell_width) + cfg::board_offset_x,
                                    int(i * cfg::cell_height) + cfg::board_offset_y,
                                    cfg::cell_width,
                                    cfg::cell_height };

        sprite_type sprite = sprite_type::EMPTY_CLOSED;

        switch (item.state)
        {
        case cell_state::CLOSED:
          sprite = sprite_type::EMPTY_CLOSED;
          break;
        case cell_state::FLAGGED:
          sprite = sprite_type::FLAG;
          break;
        case cell_state::OPENED:
          sprite = to_sprite(item.value);
          break;
        case cell_state::DETONATED:
          sprite = sprite_type::BOMB_RED;
          break;
        default:
          break;
        }

        manager.draw(sprite, dst_rect);
      }
    }
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

  void minefield::generate(size_t x, size_t y)
  {
    std::vector<size_t> coords(width_ * height_, 0);

    std::iota(coords.begin(), coords.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(coords.begin(), coords.end(), g);

    const size_t input_pos = y * width_ + x;

    for (size_t i = 0; i < bombs_cnt_; ++i)
    {
      if (coords[i] == input_pos)
        std::swap(coords[i], coords.back());

      field_[coords[i]].value = cell_value::BOMB;
    }

    place_values_around_mines();
    print_field_to_cout(field_, width_, height_);
  }

  void minefield::reset()
  {
    mswpr::cell empty{ cell_value::EMPTY, cell_state::CLOSED };
    std::fill(field_.begin(), field_.end(), empty);

    unopened_cnt_ = width_ * height_;
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
    int cnt = 0;

    while (!cells.empty())
    {
      auto cell = cells.front();
      cells.pop();
      auto& cur_cell = field_[cell.y * width_ + cell.x];
      if (visited[cell.y * width_ + cell.x])
        continue;

      ++cnt;
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
    SDL_Log("cnt: %d\n", cnt);
    return open_cell_result::OPENED;
  }

  std::vector<cell_coord> minefield::get_neighbours(cell_coord coord)
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

}  // namespace mswpr
