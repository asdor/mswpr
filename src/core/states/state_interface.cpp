#include "core/states/state_interface.hpp"
#include "core/states/state_machine.hpp"

namespace mswpr
{
  state_interface::state_interface(state_machine& st_machine) : d_st_machine(st_machine)
  {
  }

  state_machine& state_interface::get_state_machine()
  {
    return d_st_machine;
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

    auto& field = d_st_machine.get_field();
    if (field(x, y).is_flagged())
    {
      ++d_st_machine.get_counter();
    }
    else if (field(x, y).is_closed())
    {
      --d_st_machine.get_counter();
    }

    field.toggle_flag(x, y);
  }

  bool state_interface::change_face_on_click(bool is_released, face_type pressed, face_type released)
  {
    if (is_released)
    {
      d_st_machine.set_face(released);
      return false;
    }

    d_st_machine.set_face(pressed);
    return true;
  }

}  // namespace mswpr
