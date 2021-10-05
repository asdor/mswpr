#ifndef MSWPR_ENDING_STATE_HPP
#define MSWPR_ENDING_STATE_HPP

#include "states/state_interface.hpp"

namespace mswpr
{
  class ending_state : public state_interface
  {
  public:
    ending_state(mswpr::game_engine& engine);
    ending_state(mswpr::game_engine& engine, size_t x, size_t y);

    void on_left_face_click(bool is_released);
  };
}  // namespace mswpr

#endif  // MSWPR_ENDING_STATE_HPP
