#include "gui/game_renderer.hpp"
#include "core/game_config.hpp"

#include <SDL.h>

namespace
{
  bool is_inside(const SDL_Rect& rect, int x, int y)
  {
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
  }

  constexpr mswpr::sprite_type to_sprite(mswpr::cell_value value)
  {
    const size_t index = mswpr::enum_to<size_t>(value);
    constexpr size_t empty_index = mswpr::enum_to<size_t>(mswpr::sprite_type::EMPTY_OPENED);
    return mswpr::to_enum<mswpr::sprite_type>(empty_index + index);
  }

}  // namespace

mswpr::game_renderer::game_renderer(std::string_view title, size_t xpos, size_t ypos) : face_rect_(), field_rect_()
{
  const Uint32 window_mode = 0;
  const size_t window_width = 2 * cfg::board_offset_x + cfg::cell_width * cfg::field_width;
  const size_t window_height = cfg::board_offset_y + cfg::board_offset_x + cfg::cell_height * cfg::field_height;

  window_.reset(SDL_CreateWindow(
    title.data(), static_cast<int>(xpos), static_cast<int>(ypos), window_width, window_height, window_mode));
  if (!window_)
  {
    SDL_Log("Unable to create SDL_window: %s", SDL_GetError());
    return;
  }

  SDL_Log("Window created!\n");

  renderer_.reset(SDL_CreateRenderer(window_.get(), -1, 0), mswpr::sdl_deleter{});
  if (!renderer_)
  {
    SDL_Log("Unable to create SDL_Renderer: %s", SDL_GetError());
    return;
  }

  SDL_SetRenderDrawColor(renderer_.get(), 190, 190, 190, 0);
  SDL_Log("Renderer created!\n");

  texture_manager_.init(renderer_, "assets/faces.png", "assets/tile.png");

  const int face_x = cfg::field_width * cfg::cell_width / 2 - cfg::face_width / 2 + cfg::board_offset_x;
  face_rect_ = { face_x, 0, cfg::face_width, cfg::face_height };

  field_rect_ = {
    cfg::board_offset_x, cfg::board_offset_y, cfg::cell_width * cfg::field_width, cfg::cell_height * cfg::field_height
  };
}

bool mswpr::game_renderer::is_inside_face(int mouse_x, int mouse_y)
{
  return is_inside(face_rect_, mouse_x, mouse_y);
}

bool mswpr::game_renderer::is_inside_field(int mouse_x, int mouse_y)
{
  return is_inside(field_rect_, mouse_x, mouse_y);
}

void mswpr::game_renderer::render(const mswpr::minefield& field, mswpr::face_type face)
{
  SDL_RenderClear(renderer_.get());

  texture_manager_.draw(face, face_rect_);
  draw_field(field);

  SDL_RenderPresent(renderer_.get());
}

void mswpr::game_renderer::draw_field(const mswpr::minefield& field)
{
  for (size_t y = 0; y < cfg::field_height; ++y)
  {
    for (size_t x = 0; x < cfg::field_width; ++x)
    {
      const SDL_Rect dst_rect = { int(x * cfg::cell_width) + cfg::board_offset_x,
                                  int(y * cfg::cell_height) + cfg::board_offset_y,
                                  cfg::cell_width,
                                  cfg::cell_height };

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
        sprite = to_sprite(field.get_cell_value(x, y));
        break;
      case cell_state::DETONATED:
        sprite = sprite_type::BOMB_RED;
        break;
      default:
        break;
      }

      texture_manager_.draw(sprite, dst_rect);
    }
  }
}
