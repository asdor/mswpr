#ifndef MSWPR_TYPES_HPP
#define MSWPR_TYPES_HPP

#include <cstddef>
#include <type_traits>

namespace mswpr
{
  template<class T>
  concept Enumeration = std::is_enum_v<T>;

  template<class To, Enumeration E>
  constexpr To enum_to(E value)
  {
    return static_cast<To>(value);
  }

  template<Enumeration E, class From>
  constexpr E to_enum(From value)
  {
    return static_cast<E>(value);
  }

  enum class sprite_type : size_t
  {
    EMPTY_OPENED = 0,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    EMPTY_CLOSED,
    BOMB,
    FLAG,
    BOMB_FAILED,
    BOMB_RED,

    SPRITES_COUNT
  };

  inline constexpr std::size_t SPRITES_COUNT = enum_to<std::size_t>(sprite_type::SPRITES_COUNT);

  enum class face_type : size_t
  {
    SMILE_NOT_PRESSED = 0,
    WAITING,
    DEAD,
    BOSS,
    SMILE_PRESSED,

    FACES_COUNT
  };

  inline constexpr std::size_t FACES_COUNT = enum_to<std::size_t>(face_type::FACES_COUNT);

  enum class display_digits_type : size_t
  {
    EMPTY_DISPLAY,
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    MINUS,

    DIPLAY_DIGITS_COUNT
  };

  inline constexpr std::size_t DIPLAY_DIGITS_COUNT = enum_to<std::size_t>(display_digits_type::DIPLAY_DIGITS_COUNT);

  enum class border_type : size_t
  {
    TOP_LEFT,
    TOP_RIGHT,
    HUD_TOP,
    HUD_LEFT,
    HUD_RIGHT,
    DIVIDER_LEFT,
    DIVIDER_MIDDLE,
    DIVIDER_RIGHT,
    FIELD_LEFT,
    FIELD_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    FIELD_BOTTOM,

    BORDER_TYPE_COUNT
  };

  inline constexpr std::size_t BORDER_TYPE_COUNT = enum_to<std::size_t>(border_type::BORDER_TYPE_COUNT);

}  // namespace mswpr

#endif  // MSWPR_TYPES_HPP
