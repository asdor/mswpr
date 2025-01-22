#ifndef MSWPR_GAME_CONFIG_HPP
#define MSWPR_GAME_CONFIG_HPP

#include <cstddef>

namespace cfg
{
  inline constexpr size_t FIELD_WIDTH = 8;
  inline constexpr size_t FIELD_HEIGHT = 8;
  inline constexpr size_t MINES_CNT = 10;

  inline constexpr size_t FPS_LIMIT = 60;
  inline constexpr size_t FRAME_DELAY = 1000 / FPS_LIMIT;
}  // namespace cfg

namespace mswpr::layout
{
  inline constexpr size_t FACE_WIDTH = 26;
  inline constexpr size_t FACE_HEIGHT = 26;

  inline constexpr int BOARD_OFFSET_X = 12;
  inline constexpr int BOARD_TOP_HEIGHT = 11;
  inline constexpr int BOARD_BOTTOM_HEIGHT = 12;
  inline constexpr int BOARD_OFFSET_Y = 55;
  inline constexpr size_t COUNTER_OFFSET_X = 16;
  inline constexpr size_t HUD_OFFSET_Y = 15;

  inline constexpr size_t CELL_WIDTH = 16;
  inline constexpr size_t CELL_HEIGHT = 16;

  inline constexpr size_t COUNTER_WIDTH = 41;
  inline constexpr size_t COUNTER_HEIGHT = 25;
  inline constexpr size_t DIGIT_WIDTH = 11;
  inline constexpr size_t DIGIT_HEIGHT = 21;
  inline constexpr size_t DIGIT_OFFSET = 2;
}  // namespace mswpr::layout

#endif  // MSWPR_GAME_CONFIG_HPP
