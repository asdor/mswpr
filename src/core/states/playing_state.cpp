#include "core/states/playing_state.hpp"
#include "core/states/ending_state.hpp"
#include "core/states/state_machine.hpp"

namespace mswpr
{
  playing_state::playing_state(state_machine& st_machine) : state_interface(st_machine)
  {
    st_machine_.get_timer().start(game_timer::now());
  }

  void playing_state::on_left_face_click(bool is_released)
  {
    if (change_face_on_click(is_released, face_type::SMILE_PRESSED, face_type::SMILE_NOT_PRESSED))
      return;

    st_machine_.set_state<generating_state>();
  }

  void playing_state::on_left_field_click(bool is_released, size_t x, size_t y)
  {
    if (change_face_on_click(is_released, face_type::WAITING, face_type::SMILE_NOT_PRESSED))
      return;

    auto& field = st_machine_.get_field();

    if (field.reveal_closed(x, y) == open_cell_result::DETONATED)
    {
      field.reveal_bombs();
      st_machine_.set_state<ending_state>(ending_state::params{ .is_victory = false });
    }
    else if (field.is_deminied())
    {
      field.flag_bombs();
      st_machine_.get_counter().reset(0);
      st_machine_.set_state<ending_state>(ending_state::params{ .is_victory = true });
    }
  }

}  // namespace mswpr
