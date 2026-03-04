#include "gui/mines_counter_renderer.hpp"
#include "core/game_config.hpp"
#include "core/mines_counter.hpp"
#include "core/types.hpp"
#include "gui/texture_manager.hpp"

#include <SDL3/SDL_rect.h>

mswpr::mines_counter_renderer::mines_counter_renderer(mswpr::texture_manager& texture_manager) :
  d_texture_manager(texture_manager)
{
}

void mswpr::mines_counter_renderer::draw(const mines_counter& counter)
{
  const SDL_Rect empty_display_rect = { .x = mswpr::layout::COUNTER_OFFSET_X,
                                        .y = mswpr::layout::HUD_OFFSET_Y,
                                        .w = mswpr::layout::COUNTER_WIDTH,
                                        .h = mswpr::layout::COUNTER_HEIGHT };
  d_texture_manager.draw(mswpr::display_digits_type::EMPTY_DISPLAY, empty_display_rect);

  const auto digits = counter.value_to_str();

  for (size_t i = 0; i < digits.size(); ++i)
  {
    const size_t rect_x = mswpr::layout::COUNTER_OFFSET_X + mswpr::layout::DIGIT_OFFSET +
                          (mswpr::layout::DIGIT_OFFSET + mswpr::layout::DIGIT_WIDTH) * i;
    const SDL_Rect first_digit_rect = { .x = static_cast<int>(rect_x),
                                        .y = mswpr::layout::DIGIT_OFFSET + mswpr::layout::HUD_OFFSET_Y,
                                        .w = mswpr::layout::DIGIT_WIDTH,
                                        .h = mswpr::layout::DIGIT_HEIGHT };

    const auto sprite = digits[i] == '-' ? display_digits_type::MINUS
                                         : to_sprite<display_digits_type, display_digits_type::ZERO>(digits[i] - '0');
    d_texture_manager.draw(sprite, first_digit_rect);
  }
}
