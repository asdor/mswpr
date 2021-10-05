#ifndef MSWPR_GAME_CONFIG_HPP
#define MSWPR_GAME_CONFIG_HPP

#include <cstddef>

namespace cfg
{
  inline constexpr size_t field_width = 16;
  inline constexpr size_t field_height = 16;
  inline constexpr size_t mines_cnt = 1;

  inline constexpr size_t face_width = 48;
  inline constexpr size_t face_height = 48;

  inline constexpr int board_offset_x = 10;
  inline constexpr int board_offset_y = 50;
  inline constexpr size_t cell_width = 30;
  inline constexpr size_t cell_height = 30;

  inline constexpr size_t fps = 60;
  inline constexpr size_t frame_delay = 1000 / fps;

}  // namespace cfg

#endif  // MSWPR_GAME_CONFIG_HPP
