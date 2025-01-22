#include "gui/border_renderer.hpp"
#include "core/game_config.hpp"

#include <SDL_rect.h>

mswpr::border_renderer::border_renderer(mswpr::texture_manager& i_texture_manager) :
  d_texture_manager(i_texture_manager)
{
}

void mswpr::border_renderer::draw()
{
  using enum mswpr::border_type;

  const auto hud_border_height = 2 * 4 + mswpr::layout::COUNTER_HEIGHT;
  const auto field_size = mswpr::layout::CELL_HEIGHT * cfg::FIELD_HEIGHT;

  int x_pos = 0;
  draw_row(x_pos, mswpr::layout::BOARD_TOP_HEIGHT, TOP_LEFT, HUD_TOP, TOP_RIGHT);

  x_pos += mswpr::layout::BOARD_TOP_HEIGHT;
  draw_row(x_pos, hud_border_height, HUD_LEFT, std::nullopt, HUD_RIGHT);

  x_pos += hud_border_height;
  draw_row(x_pos, mswpr::layout::BOARD_TOP_HEIGHT, DIVIDER_LEFT, DIVIDER_MIDDLE, DIVIDER_RIGHT);

  x_pos += mswpr::layout::BOARD_TOP_HEIGHT;
  draw_row(x_pos, field_size, FIELD_LEFT, std::nullopt, FIELD_RIGHT);

  x_pos += field_size;
  draw_row(x_pos, mswpr::layout::BOARD_BOTTOM_HEIGHT, BOTTOM_LEFT, FIELD_BOTTOM, BOTTOM_RIGHT);
}

void mswpr::border_renderer::draw_row(std::size_t i_y,
                                      std::size_t i_h,
                                      mswpr::border_type i_left,
                                      std::optional<mswpr::border_type> i_middle,
                                      mswpr::border_type i_right)
{
  const auto y_int = static_cast<int>(i_y);
  const auto h_int = static_cast<int>(i_h);
  const size_t window_width =
    static_cast<size_t>(2) * mswpr::layout::BOARD_OFFSET_X + mswpr::layout::CELL_WIDTH * cfg::FIELD_WIDTH;

  const SDL_Rect left_rect = { .x = 0, .y = y_int, .w = mswpr::layout::BOARD_OFFSET_X, .h = h_int };
  d_texture_manager.draw(i_left, left_rect);

  if (i_middle)
  {
    const SDL_Rect middle_rect = {
      .x = mswpr::layout::BOARD_OFFSET_X, .y = y_int, .w = mswpr::layout::CELL_WIDTH * cfg::FIELD_WIDTH, .h = h_int
    };
    d_texture_manager.draw(*i_middle, middle_rect);
  }

  const SDL_Rect right_rect = {
    .x = window_width - mswpr::layout::BOARD_OFFSET_X, .y = y_int, .w = mswpr::layout::BOARD_OFFSET_X, .h = h_int
  };
  d_texture_manager.draw(i_right, right_rect);
}
