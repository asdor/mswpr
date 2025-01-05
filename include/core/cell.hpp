#ifndef MSWPR_CELL_HPP
#define MSWPR_CELL_HPP

#include <cstddef>
#include <type_traits>

namespace mswpr
{
  enum class cell_value : std::size_t
  {
    EMPTY = 0,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    BOMB
  };

  enum class cell_state
  {
    CLOSED,
    OPENED,
    FLAGGED,
    DETONATED,
    NOT_FLAGGED_BOMB
  };

  struct cell
  {
    cell_value value;
    cell_state state;

    bool is_empty() const;
    bool is_bomb() const;

    bool is_closed() const;
    bool is_opened() const;
    bool is_flagged() const;
    bool is_detonated() const;
    bool is_not_flagged_bomb() const;
  };
  static_assert(std::is_trivial_v<cell>);

}  // namespace mswpr

#endif  // MSWPR_CELL_HPP
