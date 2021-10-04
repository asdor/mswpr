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
    // engine_.set_face(face_type::SMILE_OPENED);
    engine_.set_state<generating_state>();
  }

  void playing_state::on_left_field_click(bool is_released, size_t x, size_t y)
  {
    if (change_face_on_click(is_released, face_type::WAITING, face_type::SMILE_CLOSED))
      return;

    auto& field = engine_.get_field();
    /*if (field.is_bomb(x, y))
    {
      engine_.set_state<ending_state>(x, y);
      return;
    }
    else if (!field.open_cell(x, y) || (field.get_value(x, y) > 0))
    {
      return;
    }*/

    if (field.reveal_closed(x, y) == open_cell_result::DETONATED)
    {
      engine_.set_state<ending_state>(x, y);
      // field.detonate_bomb(x, y);
    }

    // field.reveal_closed(x, y);
  }

  void playing_state::on_right_field_click(bool is_released, size_t x, size_t y)
  {
    if (!is_released)
      return;

    auto& field = engine_.get_field();
    field.set_flag(x, y);
  }

}  // namespace mswpr
