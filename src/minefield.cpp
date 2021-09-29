#include <algorithm>
#include <random>
#include <stack>
#include <numeric>

#include "game_config.hpp"
#include "minefield.hpp"
#include "texture_manager.hpp"
#include "types.hpp"

namespace
{
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
    // generate();
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

  void minefield::generate()
  {
    // auto bomb = [this](size_t x, size_t y) {
    //     field_[y * width_ + x].value = cell_value::BOMB;
    // };
    // bomb(3, 0);
    // bomb(7, 0);
    // bomb(2, 1);
    // bomb(3, 1);
    // bomb(6, 1);
    // bomb(0, 2);
    // bomb(6, 3);
    // bomb(7, 4);
    // bomb(4, 6);
    // bomb(5, 6);

    std::vector<size_t> coords(width_ * height_, 0);

    std::iota(coords.begin(), coords.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(coords.begin(), coords.end(), g);

    for (size_t i = 0; i < bombs_cnt_; ++i)
      field_[coords[i]].value = cell_value::BOMB;

    static constexpr std::array<int, 8> dir_x = { -1, 0, 1, -1, 1, -1, 0, 1 };
    static constexpr std::array<int, 8> dir_y = { -1, -1, -1, 0, 0, 1, 1, 1 };

    const int width_i = static_cast<int>(width_);
    const int height_i = static_cast<int>(height_);
    for (int y = 0; y < width_i; ++y)
    {
      for (int x = 0; x < height_i; ++x)
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

  void minefield::on_left_click(size_t mouse_x, size_t mouse_y)
  {
    const size_t x = (mouse_x - cfg::board_offset_x) / cfg::cell_width;
    const size_t y = (mouse_y - cfg::board_offset_y) / cfg::cell_height;
    SDL_Log("Left mouse click at (%ld, %ld)", x, y);
    if (x >= width_ || y >= height_)
      return;

    /*auto& elem = field_[y * width_ + x];*/
    if (auto& elem = field_[y * width_ + x]; elem.is_closed() && elem.is_empty())
      reveal_closed(x, y);
    else if (elem.is_closed())
      elem.state = cell_state::OPENED;
  }

  void minefield::on_right_click(size_t mouse_x, size_t mouse_y)
  {
    const size_t x = (mouse_x - cfg::board_offset_x) / cfg::cell_width;
    const size_t y = (mouse_y - cfg::board_offset_y) / cfg::cell_height;
    // SDL_Log("Right mouse click at (%ld, %ld)", x, y);
    if (x >= width_ || y >= height_)
      return;

    auto& elem = field_[y * width_ + x];
    if (elem.is_closed())
      elem.state = cell_state::FLAGGED;
    else if (elem.is_flagged())
      elem.state = cell_state::CLOSED;
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
