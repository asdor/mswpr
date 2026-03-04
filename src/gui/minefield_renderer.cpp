#include "gui/minefield_renderer.hpp"
#include "core/game_config.hpp"
#include "core/minefield.hpp"
#include "core/types.hpp"
#include "gui/texture_manager.hpp"

#include <SDL3/SDL_rect.h>

mswpr::minefield_renderer::minefield_renderer(mswpr::texture_manager& texture_manager) :
  d_texture_manager(texture_manager)
{
}

void mswpr::minefield_renderer::draw(const minefield& field)
{
  for (size_t y = 0; y < cfg::FIELD_HEIGHT; ++y)
  {
    for (size_t x = 0; x < cfg::FIELD_WIDTH; ++x)
    {
      const auto cell_x = static_cast<int>(x * mswpr::layout::CELL_WIDTH) + mswpr::layout::BORDER_WIDTH;
      const auto cell_y = static_cast<int>(y * mswpr::layout::CELL_HEIGHT) + mswpr::layout::BOARD_OFFSET_Y;
      const SDL_Rect dst_rect = {
        .x = cell_x, .y = cell_y, .w = mswpr::layout::CELL_WIDTH, .h = mswpr::layout::CELL_HEIGHT
      };

      sprite_type sprite = sprite_type::EMPTY_CLOSED;
      auto state = field.get_cell_state(x, y);

      switch (state)
      {
      case cell_state::CLOSED:
        sprite = sprite_type::EMPTY_CLOSED;
        break;
      case cell_state::FLAGGED:
        sprite = sprite_type::FLAG;
        break;
      case cell_state::OPENED:
        sprite = to_sprite<sprite_type, sprite_type::EMPTY_OPENED>(field.get_cell_value(x, y));
        break;
      case cell_state::DETONATED:
        sprite = sprite_type::BOMB_RED;
        break;
      case cell_state::NOT_FLAGGED_BOMB:
        sprite = sprite_type::BOMB_FAILED;
        break;
      default:
        break;
      }

      d_texture_manager.draw(sprite, dst_rect);
    }
  }
}
