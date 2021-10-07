#include "core/states/state_machine.hpp"
#include "core/states/ending_state.hpp"
#include "core/states/generating_state.hpp"

namespace mswpr
{
  ending_state::ending_state(mswpr::state_machine& st_machine) : state_interface(st_machine)
  {
    st_machine_.set_face(face_type::BOSS);
  }

  ending_state::ending_state(mswpr::state_machine& st_machine, size_t x, size_t y) : state_interface(st_machine)
  {
    st_machine_.set_face(face_type::DEAD);
  }

  void ending_state::on_left_face_click(bool is_released)
  {
    if (change_face_on_click(is_released, face_type::SMILE_OPENED, face_type::DEAD))
      return;

    //SDL_Log("ending_state");
    st_machine_.set_state<generating_state>();
  }
}  // namespace mswpr
