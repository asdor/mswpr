#include "game_engine.hpp"
#include "states/generating_state.hpp"
#include "states/playing_state.hpp"

namespace mswpr
{
  generating_state::generating_state(mswpr::state_machine& st_machine) : state_interface(st_machine)
  {
    st_machine_.set_face(face_type::SMILE_CLOSED);
    st_machine_.get_field().reset();
  }

  void generating_state::on_left_face_click(bool is_released)
  {
    if (change_face_on_click(is_released, face_type::SMILE_OPENED, face_type::SMILE_CLOSED))
      return;

    st_machine_.get_field().reset();
    SDL_Log("generating_state");
  }

  void generating_state::on_left_field_click(bool is_released, size_t x, size_t y)
  {
    if (change_face_on_click(is_released, face_type::WAITING, face_type::SMILE_CLOSED))
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
    field.set_flag(x, y);
  }
}  // namespace mswpr
