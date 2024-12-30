#include "gui/game_renderer.hpp"
#include "gui/game_version.hpp"
#include "core/game_config.hpp"

#include <SDL.h>

#include <concepts>

namespace
{
  bool is_inside(const SDL_Rect& rect, int x, int y)
  {
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
  }

  template<mswpr::Enumeration To, To base, std::integral Index>
  constexpr To to_sprite(Index index_integral)
  {
    const size_t index = static_cast<size_t>(index_integral);
    constexpr size_t empty_index = mswpr::enum_to<size_t>(base);
    return mswpr::to_enum<To>(empty_index + index);
  }

  template<mswpr::Enumeration To, To base, mswpr::Enumeration From>
  constexpr To to_sprite(From value)
  {
    const size_t index = mswpr::enum_to<size_t>(value);
    return to_sprite<To, base>(index);
  }

}  // namespace

mswpr::game_renderer::game_renderer(std::string_view title, size_t xpos, size_t ypos) : face_rect_(), field_rect_()
{
  SDL_Log("sdl2_minesweeper version: %s\n", mswpr::get_game_version());

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

  texture_manager_.init(renderer_, "assets/winxpskin.bmp");

  const int face_x = cfg::field_width * cfg::cell_width / 2 - cfg::face_width / 2 + cfg::board_offset_x;
  face_rect_ = { face_x, cfg::hud_offset_y, cfg::face_width, cfg::face_height };

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

void mswpr::game_renderer::render(const mswpr::minefield& field,
                                  mswpr::face_type face,
                                  const mswpr::mines_counter& counter,
                                  const mswpr::game_timer& i_timer)
{
  SDL_RenderClear(renderer_.get());

  draw_mines_counter(counter);
  draw_timer(i_timer);
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

      texture_manager_.draw(sprite, dst_rect);
    }
  }
}

void mswpr::game_renderer::draw_mines_counter(const mswpr::mines_counter& counter)
{
  const SDL_Rect empty_display_rect = {
    .x = cfg::counter_offset_x, .y = cfg::hud_offset_y, .w = cfg::counter_width, .h = cfg::counter_height
  };
  texture_manager_.draw(mswpr::display_digits_type::EMPTY_DISPLAY, empty_display_rect);

  const auto digits = counter.value_to_str();

  for (size_t i = 0; i < digits.size(); ++i)
  {
    const size_t rect_x = cfg::counter_offset_x + cfg::digit_offset + (cfg::digit_offset + cfg::digit_width) * i;
    const SDL_Rect first_digit_rect = { .x = static_cast<int>(rect_x),
                                        .y = cfg::digit_offset + cfg::hud_offset_y,
                                        .w = cfg::digit_width,
                                        .h = cfg::digit_height };

    display_digits_type sprite = digits[i] == '-'
                                   ? display_digits_type::MINUS
                                   : to_sprite<display_digits_type, display_digits_type::ZERO>(digits[i] - '0');
    texture_manager_.draw(sprite, first_digit_rect);
  }
}

void mswpr::game_renderer::draw_timer(const mswpr::game_timer& i_timer)
{
  const size_t window_width = 2 * cfg::board_offset_x + cfg::cell_width * cfg::field_width;
  const size_t x_timer = window_width - cfg::counter_offset_x - cfg::counter_width - 2;
  const SDL_Rect empty_display_rect = {
    .x = static_cast<int>(x_timer), .y = cfg::hud_offset_y, .w = cfg::counter_width, .h = cfg::counter_height
  };
  texture_manager_.draw(mswpr::display_digits_type::EMPTY_DISPLAY, empty_display_rect);

  const auto digits = i_timer.as_digit_array();

  for (size_t i = 0; i < digits.size(); ++i)
  {
    const size_t rect_x = x_timer + cfg::digit_offset + (cfg::digit_offset + cfg::digit_width) * i;
    const SDL_Rect first_digit_rect = { .x = static_cast<int>(rect_x),
                                        .y = cfg::digit_offset + cfg::hud_offset_y,
                                        .w = cfg::digit_width,
                                        .h = cfg::digit_height };

    const display_digits_type sprite = to_sprite<display_digits_type, display_digits_type::ZERO>(digits[i]);
    texture_manager_.draw(sprite, first_digit_rect);
  }
}
