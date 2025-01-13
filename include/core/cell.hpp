#ifndef MSWPR_CELL_HPP
#define MSWPR_CELL_HPP

#include <compare>
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
  public:
    constexpr cell() noexcept : d_value(cell_value::EMPTY), d_state(cell_state::CLOSED)
    {
    }

    constexpr cell(cell_value i_value) noexcept : d_value(i_value), d_state(cell_state::CLOSED)
    {
    }

    cell_value get_value() const;
    cell_state get_state() const;
    void set_state(cell_state i_state);

    bool is_empty() const;
    bool is_bomb() const;

    bool is_closed() const;
    bool is_opened() const;
    bool is_flagged() const;
    bool is_detonated() const;
    bool is_not_flagged_bomb() const;

  private:
    cell_value d_value;
    cell_state d_state;
  };
  static_assert(std::is_trivially_copyable_v<cell>);
  static_assert(std::is_trivially_move_constructible_v<cell>);

  struct cell_coord
  {
    size_t x;
    size_t y;

    auto operator<=>(const cell_coord&) const = default;
  };

}  // namespace mswpr

#endif  // MSWPR_CELL_HPP
