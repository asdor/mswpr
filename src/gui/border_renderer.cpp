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

  const auto hud_inner_height = 2 * mswpr::layout::COUNTER_OFFSET_Y + mswpr::layout::COUNTER_HEIGHT;
  const auto field_size = mswpr::layout::CELL_HEIGHT * cfg::FIELD_HEIGHT;

  size_t row_y_pos = 0;
  size_t row_height = mswpr::layout::BORDER_HEIGHT_TOP;
  draw_row(row_y_pos, row_height, TOP_LEFT, HUD_TOP, TOP_RIGHT);

  row_y_pos += row_height;
  row_height = hud_inner_height;
  draw_row(row_y_pos, row_height, HUD_LEFT, std::nullopt, HUD_RIGHT);

  row_y_pos += row_height;
  row_height = mswpr::layout::DIVIDER_HEIGHT;
  draw_row(row_y_pos, row_height, DIVIDER_LEFT, DIVIDER_MIDDLE, DIVIDER_RIGHT);

  row_y_pos += row_height;
  row_height = field_size;
  draw_row(row_y_pos, row_height, FIELD_LEFT, std::nullopt, FIELD_RIGHT);

  row_y_pos += row_height;
  draw_row(row_y_pos, mswpr::layout::BORDER_HEIGHT_BOTTOM, BOTTOM_LEFT, FIELD_BOTTOM, BOTTOM_RIGHT);
}

void mswpr::border_renderer::draw_row(std::size_t i_row_y_pos,
                                      std::size_t i_row_height,
                                      mswpr::border_type i_left,
                                      std::optional<mswpr::border_type> i_middle,
                                      mswpr::border_type i_right)
{
  const auto row_y_pos_int = static_cast<int>(i_row_y_pos);
  const auto row_height_int = static_cast<int>(i_row_height);
  const size_t window_width =
    static_cast<size_t>(2) * mswpr::layout::BORDER_WIDTH + mswpr::layout::CELL_WIDTH * cfg::FIELD_WIDTH;

  const SDL_Rect left_rect = { .x = 0, .y = row_y_pos_int, .w = mswpr::layout::BORDER_WIDTH, .h = row_height_int };
  d_texture_manager.draw(i_left, left_rect);

  if (i_middle)
  {
    const SDL_Rect middle_rect = { .x = mswpr::layout::BORDER_WIDTH,
                                   .y = row_y_pos_int,
                                   .w = mswpr::layout::CELL_WIDTH * cfg::FIELD_WIDTH,
                                   .h = row_height_int };
    d_texture_manager.draw(*i_middle, middle_rect);
  }

  const SDL_Rect right_rect = { .x = window_width - mswpr::layout::BORDER_WIDTH,
                                .y = row_y_pos_int,
                                .w = mswpr::layout::BORDER_WIDTH,
                                .h = row_height_int };
  d_texture_manager.draw(i_right, right_rect);
}
