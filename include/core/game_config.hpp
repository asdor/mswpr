#ifndef MSWPR_GAME_CONFIG_HPP
#define MSWPR_GAME_CONFIG_HPP

#include <cstddef>

namespace cfg
{
  inline constexpr size_t field_width = 8;
  inline constexpr size_t field_height = 8;
  inline constexpr size_t mines_cnt = 10;

  inline constexpr size_t face_width = 26;
  inline constexpr size_t face_height = 26;

  inline constexpr int board_offset_x = 12;
  inline constexpr int board_offset_y = 55;
  inline constexpr size_t counter_offset_x = 16;
  inline constexpr size_t hud_offset_y = 15;

  inline constexpr size_t cell_width = 16;
  inline constexpr size_t cell_height = 16;

  inline constexpr size_t counter_width = 41;
  inline constexpr size_t counter_height = 25;
  inline constexpr size_t digit_width = 11;
  inline constexpr size_t digit_height = 21;
  inline constexpr size_t digit_offset = 2;

  inline constexpr size_t fps = 60;
  inline constexpr size_t frame_delay = 1000 / fps;

}  // namespace cfg

#endif  // MSWPR_GAME_CONFIG_HPP
