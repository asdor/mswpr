#ifndef MSWPR_ENDING_STATE_HPP
#define MSWPR_ENDING_STATE_HPP

#include "states/state_interface.hpp"

namespace mswpr
{
  class ending_state : public state_interface
  {
  public:
    using state_interface::state_interface;
    ending_state(mswpr::game_engine& engine, size_t x, size_t y);

    void on_left_face_click(bool is_released);
    // void on_left_field_click(size_t x, size_t y);
    // void on_right_field_click(size_t x, size_t y);
  };
}  // namespace mswpr

#endif  // MSWPR_ENDING_STATE_HPP
