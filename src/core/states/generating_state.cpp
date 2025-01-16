#include "core/states/generating_state.hpp"
#include "core/mines_generator.hpp"
#include "core/states/playing_state.hpp"
#include "core/states/state_machine.hpp"

namespace mswpr
{
  generating_state::generating_state(mswpr::state_machine& st_machine) : state_interface(st_machine)
  {
    auto& state_machine = get_state_machine();
    state_machine.set_face(face_type::SMILE_NOT_PRESSED);
    auto& field = state_machine.get_field();
    field.reset();
    state_machine.get_counter().reset(field.get_bomb_cnt());
    state_machine.get_timer().reset();
  }

  void generating_state::on_left_face_click(bool is_released)
  {
    if (change_face_on_click(is_released, face_type::SMILE_PRESSED, face_type::SMILE_NOT_PRESSED))
      return;

    get_state_machine().set_state<generating_state>();
  }

  void generating_state::on_left_field_click(bool is_released, size_t x, size_t y)
  {
    if (change_face_on_click(is_released, face_type::WAITING, face_type::SMILE_NOT_PRESSED))
    {
      return;
    }

    auto& field = get_state_machine().get_field();
    const mswpr::glade_generator glade_generator(field.get_width(), field.get_height(), field.get_bomb_cnt(), x, y);
    field.generate(glade_generator);
    field.reveal_closed(x, y);

    get_state_machine().set_state<playing_state>();
  }

}  // namespace mswpr
