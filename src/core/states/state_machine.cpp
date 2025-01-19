#include "core/states/state_machine.hpp"

namespace mswpr
{
  state_machine::state_machine(mswpr::minefield& minefield,
                               mswpr::face_type& face_type,
                               mswpr::mines_counter& counter,
                               mswpr::game_timer& timer) :
    d_minefield(minefield),
    d_face_type(face_type),
    d_counter(counter),
    d_timer(timer),
    d_state(std::in_place_type<generating_state>, *this)
  {
  }

  void state_machine::set_face(mswpr::face_type face)
  {
    d_face_type = face;
  }

  mswpr::minefield& state_machine::get_field()
  {
    return d_minefield;
  }

  mswpr::mines_counter& state_machine::get_counter()
  {
    return d_counter;
  }

  mswpr::game_timer& state_machine::get_timer()
  {
    return d_timer;
  }

  void state_machine::on_left_face_click(bool is_released)
  {
    std::visit([is_released](auto& st) { st.on_left_face_click(is_released); }, d_state);
  }

  void state_machine::on_left_field_click(bool is_released, size_t x, size_t y)
  {
    std::visit([is_released, x, y](auto& st) { st.on_left_field_click(is_released, x, y); }, d_state);
  }
  void state_machine::on_right_field_click(bool is_released, size_t x, size_t y)
  {
    std::visit([is_released, x, y](auto& st) { st.on_right_field_click(is_released, x, y); }, d_state);
  }
}  // namespace mswpr
