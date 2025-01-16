#include "core/states/ending_state.hpp"
#include "core/states/generating_state.hpp"
#include "core/states/state_machine.hpp"

namespace mswpr
{
  ending_state::ending_state(mswpr::state_machine& st_machine, ending_state::params state_params) :
    state_interface(st_machine)
  {
    get_state_machine().set_face(state_params.is_victory ? face_type::BOSS : face_type::DEAD);
    get_state_machine().get_timer().stop(game_timer::now());
  }

  void ending_state::on_left_face_click(bool is_released)
  {
    if (change_face_on_click(is_released, face_type::SMILE_PRESSED, face_type::DEAD))
      return;

    get_state_machine().set_state<generating_state>();
  }

  void ending_state::on_right_field_click(bool /* is_released */, size_t /* x */, size_t /* y */)
  {
  }
}  // namespace mswpr
