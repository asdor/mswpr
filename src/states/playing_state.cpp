#include "game_engine.hpp"
#include "states/ending_state.hpp"
#include "states/playing_state.hpp"

namespace mswpr
{
  void playing_state::on_left_face_click(bool is_released)
  {
    if (change_face_on_click(is_released, face_type::SMILE_OPENED, face_type::SMILE_CLOSED))
      return;

    SDL_Log("playing_state");
    st_machine_.set_state<generating_state>();
  }

  void playing_state::on_left_field_click(bool is_released, size_t x, size_t y)
  {
    if (change_face_on_click(is_released, face_type::WAITING, face_type::SMILE_CLOSED))
      return;

    auto& field = st_machine_.get_field();

    if (field.reveal_closed(x, y) == open_cell_result::DETONATED)
    {
      st_machine_.set_state<ending_state>(x, y);
    }
    else if (field.is_deminied())
    {
      st_machine_.set_state<ending_state>();
    }
  }

  void playing_state::on_right_field_click(bool is_released, size_t x, size_t y)
  {
    if (!is_released)
      return;

    auto& field = st_machine_.get_field();
    field.set_flag(x, y);
  }

}  // namespace mswpr
