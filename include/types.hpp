#ifndef MSWPR_TYPES_HPP
#define MSWPR_TYPES_HPP

#include <cstddef>

namespace minesweeper
{
enum class sprite_type : size_t
{
    EMPTY = 0,
    EMPTY_PRESSED,
    FLAG,
    QUESTION,
    QUESTION_PRESSED,
    BOMB,
    BOMB_RED,
    BOMB_FAILED,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT
};

inline constexpr std::size_t SPRITES_COUNT = 16;

} // namespace minesweeper

#endif // MSWPR_TYPES_HPP
