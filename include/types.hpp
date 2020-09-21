#ifndef MSWPR_TYPES_HPP
#define MSWPR_TYPES_HPP

#include <cstddef>
#include <type_traits>

namespace mswpr
{
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
    BOMB,
    EMPTY_CLOSED,
    FLAG,
    QUESTION,
    QUESTION_OPENED,
    BOMB_RED,
    BOMB_FAILED
};

inline constexpr std::size_t SPRITES_COUNT = 16;

template <class T>
concept Enumeration = std::is_enum_v<T>;

template <class To, Enumeration E>
constexpr To enum_to(E value)
{
    return static_cast<To>(value);
}

template <Enumeration E, class From>
constexpr E to_enum(From value)
{
    return static_cast<E>(value);
}

} // namespace mswpr

#endif // MSWPR_TYPES_HPP
