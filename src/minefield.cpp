#include <algorithm>
#include <random>
#include <stack>
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

  minefield::minefield(size_t width, size_t height, size_t bombs_cnt) :
    width_(width),
    height_(height),
    bombs_cnt_(bombs_cnt),
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
        default:
          break;
        }

        manager.draw(sprite, dst_rect);
      }
    }
  }

  void minefield::place_values_around_mines()
  {
    static constexpr std::array<int, 8> dir_x = { -1, 0, 1, -1, 1, -1, 0, 1 };
    static constexpr std::array<int, 8> dir_y = { -1, -1, -1, 0, 0, 1, 1, 1 };

    const int width_i = static_cast<int>(width_);
    const int height_i = static_cast<int>(height_);
    for (int y = 0; y < height_i; ++y)
    {
      for (int x = 0; x < width_i; ++x)
      {
        if (field_[y * width_ + x].value == cell_value::BOMB)
          continue;

        size_t cnt = 0;
        for (size_t i = 0; i < dir_x.size(); ++i)
        {
          int i_x = x - dir_x[i];
          int i_y = y - dir_y[i];
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
          cell_name = '1' + (static_cast<size_t>(cell) - static_cast<size_t>(mswpr::cell_value::ONE));
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

  void minefield::reveal_closed(size_t x, size_t y)
  {
    struct Coord
    {
      int x;
      int y;
    };

    std::vector<int> visited(width_ * height_, 0);

    std::stack<Coord> cells;
    cells.push({ static_cast<int>(x), static_cast<int>(y) });

    static constexpr std::array<int, 8> dir_x = { -1, 0, 1, -1, 1, -1, 0, 1 };
    static constexpr std::array<int, 8> dir_y = { -1, -1, -1, 0, 0, 1, 1, 1 };
    const int width_i = static_cast<int>(width_);
    const int height_i = static_cast<int>(height_);

    size_t cnt = 0;

    while (!cells.empty())
    {
      if (cnt == 100)
      {
        SDL_Log("BREAKED!");
        break;
      }

      auto c = cells.top();
      cells.pop();

      if (field_[c.y * width_ + c.x].is_closed())
        field_[c.y * width_ + c.x].state = cell_state::OPENED;
      visited[c.y * width_ + c.x] = 1;

      for (size_t i = 0; i < dir_x.size(); ++i)
      {
        int i_x = c.x + dir_x[i];
        int i_y = c.y + dir_y[i];
        if (i_x >= 0 && i_x < width_i && i_y >= 0 && i_y < height_i)
        {
          auto& elem = field_[i_y * width_ + i_x];
          if (elem.is_empty() && !elem.is_flagged() && visited[i_y * width_ + i_x] != 1)
            cells.push({ i_x, i_y });
          else if (elem.is_closed())
            elem.state = cell_state::OPENED;
        }
      }

      ++cnt;
    }
  }

  void minefield::reset()
  {
    mswpr::cell empty{ cell_value::EMPTY, cell_state::CLOSED };
    std::fill(field_.begin(), field_.end(), empty);
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

  bool minefield::open_cell(size_t x, size_t y)
  {
    auto& cell = field_[y * width_ + x];
    if (!cell.is_closed())
      return false;

    cell.state = cell_state::OPENED;
    return true;
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
}  // namespace mswpr
