#include "core/cell.hpp"

namespace mswpr
{
  bool cell::is_empty() const
  {
    return value == cell_value::EMPTY;
  }

  bool cell::is_bomb() const
  {
    return value == cell_value::BOMB;
  }

  bool cell::is_closed() const
  {
    return state == cell_state::CLOSED;
  }

  bool cell::is_opened() const
  {
    return state == cell_state::OPENED;
  }

  bool cell::is_flagged() const
  {
    return state == cell_state::FLAGGED;
  }

  bool cell::is_detonated() const
  {
    return state == cell_state::DETONATED;
  }

  bool cell::is_not_flagged_bomb() const
  {
    return state == cell_state::NOT_FLAGGED_BOMB;
  }
}
