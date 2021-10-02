#ifndef MSWPR_GAME_CONFIG_HPP
#define MSWPR_GAME_CONFIG_HPP

#include <cstddef>

namespace cfg
{
  inline constexpr size_t field_width = 8;
  inline constexpr size_t field_height = 8;
  inline constexpr size_t mines_cnt = 10;

  inline constexpr size_t face_width = 48;
  inline constexpr size_t face_height = 48;

  inline constexpr int board_offset_x = 10;
  inline constexpr int board_offset_y = 50;
  inline constexpr size_t cell_width = 30;
  inline constexpr size_t cell_height = 30;

}  // namespace cfg

#endif  // MSWPR_GAME_CONFIG_HPP
