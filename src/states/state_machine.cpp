#include "states/state_machine.hpp"

namespace mswpr
{
  state_machine::state_machine(mswpr::minefield& minefield, mswpr::face_type& face_type) :
    minefield_(minefield),
    face_type_(face_type),
    state_(std::in_place_type<generating_state>, *this)
  {
  }

  void state_machine::set_face(mswpr::face_type face)
  {
    face_type_ = face;
  }

  mswpr::minefield& state_machine::get_field()
  {
    return minefield_;
  }

  void state_machine::on_left_face_click(bool is_released)
  {
    std::visit([is_released](auto& st) { st.on_left_face_click(is_released); }, state_);
  }

  void state_machine::on_left_field_click(bool is_released, size_t x, size_t y)
  {
    std::visit([is_released, x, y](auto& st) { st.on_left_field_click(is_released, x, y); }, state_);
  }
  void state_machine::on_right_field_click(bool is_released, size_t x, size_t y)
  {
    std::visit([is_released, x, y](auto& st) { st.on_right_field_click(is_released, x, y); }, state_);
  }
}
