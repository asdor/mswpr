#include "core/cell.hpp"

namespace mswpr
{
  cell_value cell::get_value() const
  {
    return d_value;
  }

  cell_state cell::get_state() const
  {
    return d_state;
  }

  void cell::set_state(cell_state i_state)
  {
    d_state = i_state;
  }

  bool cell::is_empty() const
  {
    return d_value == cell_value::EMPTY;
  }

  bool cell::is_bomb() const
  {
    return d_value == cell_value::BOMB;
  }

  bool cell::is_closed() const
  {
    return d_state == cell_state::CLOSED;
  }

  bool cell::is_opened() const
  {
    return d_state == cell_state::OPENED;
  }

  bool cell::is_flagged() const
  {
    return d_state == cell_state::FLAGGED;
  }

  bool cell::is_detonated() const
  {
    return d_state == cell_state::DETONATED;
  }

  bool cell::is_not_flagged_bomb() const
  {
    return d_state == cell_state::NOT_FLAGGED_BOMB;
  }
}  // namespace mswpr
