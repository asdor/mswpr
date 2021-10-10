#ifndef MSWPR_GENERATING_STATE_HPP
#define MSWPR_GENERATING_STATE_HPP

#include "core/states/state_interface.hpp"

namespace mswpr
{
  class generating_state : public state_interface
  {
  public:
    generating_state(mswpr::state_machine& st_machine);

    void on_left_face_click(bool is_released);
    void on_left_field_click(bool is_released, size_t x, size_t y);
  };
}  // namespace mswpr

#endif  // MSWPR_GENERATING_STATE_HPP
