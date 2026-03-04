#include "gui/game_timer_renderer.hpp"
#include "core/game_config.hpp"
#include "core/game_timer.hpp"
#include "core/types.hpp"
#include "gui/texture_manager.hpp"

#include <SDL3/SDL_rect.h>

mswpr::game_timer_renderer::game_timer_renderer(mswpr::texture_manager& texture_manager) :
  d_texture_manager(texture_manager)
{
}

void mswpr::game_timer_renderer::draw(const game_timer& timer)
{
  const size_t window_width =
    static_cast<size_t>(2) * mswpr::layout::BORDER_WIDTH + mswpr::layout::CELL_WIDTH * cfg::FIELD_WIDTH;
  const size_t x_timer = window_width - mswpr::layout::COUNTER_OFFSET_X - mswpr::layout::COUNTER_WIDTH - 2;
  const SDL_Rect empty_display_rect = { .x = static_cast<int>(x_timer),
                                        .y = mswpr::layout::HUD_OFFSET_Y,
                                        .w = mswpr::layout::COUNTER_WIDTH,
                                        .h = mswpr::layout::COUNTER_HEIGHT };
  d_texture_manager.draw(mswpr::display_digits_type::EMPTY_DISPLAY, empty_display_rect);

  const auto digits = timer.extract_digits_from_seconds();

  for (size_t i = 0; i < digits.size(); ++i)
  {
    const size_t rect_x =
      x_timer + mswpr::layout::DIGIT_OFFSET + (mswpr::layout::DIGIT_OFFSET + mswpr::layout::DIGIT_WIDTH) * i;
    const SDL_Rect first_digit_rect = { .x = static_cast<int>(rect_x),
                                        .y = mswpr::layout::DIGIT_OFFSET + mswpr::layout::HUD_OFFSET_Y,
                                        .w = mswpr::layout::DIGIT_WIDTH,
                                        .h = mswpr::layout::DIGIT_HEIGHT };

    const auto sprite = to_sprite<display_digits_type, display_digits_type::ZERO>(digits[i]);
    d_texture_manager.draw(sprite, first_digit_rect);
  }
}
