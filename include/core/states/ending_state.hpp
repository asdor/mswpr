#ifndef MSWPR_ENDING_STATE_HPP
#define MSWPR_ENDING_STATE_HPP

#include "core/states/state_interface.hpp"

namespace mswpr
{

  class ending_state : public state_interface
  {
  public:
    struct params
    {
      bool is_victory;
    };

    ending_state(mswpr::state_machine& st_machine, ending_state::params state_params);

    void on_left_face_click(bool is_released);
    void on_right_field_click(bool is_released, size_t x, size_t y);
  };
}  // namespace mswpr

#endif  // MSWPR_ENDING_STATE_HPP
