#include "core/states/state_machine.hpp"
#include "core/states/generating_state.hpp"
#include "core/states/playing_state.hpp"

namespace mswpr
{
  generating_state::generating_state(mswpr::state_machine& st_machine) : state_interface(st_machine)
  {
    st_machine_.set_face(face_type::SMILE_NOT_PRESSED);
    auto& field = st_machine_.get_field();
    field.reset();
    st_machine_.get_counter().reset(field.get_bomb_cnt());
  }

  void generating_state::on_left_face_click(bool is_released)
  {
    if (change_face_on_click(is_released, face_type::SMILE_PRESSED, face_type::SMILE_NOT_PRESSED))
      return;

    st_machine_.set_state<generating_state>();
  }

  void generating_state::on_left_field_click(bool is_released, size_t x, size_t y)
  {
    if (change_face_on_click(is_released, face_type::WAITING, face_type::SMILE_NOT_PRESSED))
    {
      return;
    }

    auto& field = st_machine_.get_field();
    field.generate(x, y);
    field.reveal_closed(x, y);

    st_machine_.set_state<playing_state>();
  }

  void generating_state::on_right_field_click(bool is_released, size_t x, size_t y)
  {
    if (!is_released)
      return;

    auto& field = st_machine_.get_field();
    if (field.is_flagged(x, y))
    {
      ++st_machine_.get_counter();
    }
    else if (field.is_closed(x, y))
    {
      --st_machine_.get_counter();
    }

    field.set_flag(x, y);
  }
}  // namespace mswpr
