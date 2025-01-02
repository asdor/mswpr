#include "core/states/state_interface.hpp"
#include "core/states/state_machine.hpp"

namespace mswpr
{
  state_interface::state_interface(state_machine& st_machine) : st_machine_(st_machine)
  {
  }

  void state_interface::on_left_face_click(bool /*is_released*/)
  {
  }

  void state_interface::on_left_field_click(bool /*is_released*/, size_t, size_t)
  {
  }

  void state_interface::on_right_field_click(bool is_released, size_t x, size_t y)
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

  bool state_interface::change_face_on_click(bool is_released, face_type pressed, face_type released)
  {
    if (is_released)
    {
      st_machine_.set_face(released);
      return false;
    }

    st_machine_.set_face(pressed);
    return true;
  }

}  // namespace mswpr
