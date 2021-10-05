#include "game_engine.hpp"
#include "states/generating_state.hpp"
#include "states/playing_state.hpp"

namespace mswpr
{
  generating_state::generating_state(mswpr::game_engine& engine) : state_interface(engine)
  {
    engine_.get_field().reset();
  }

  void generating_state::on_left_face_click(bool is_released)
  {
    if (change_face_on_click(is_released, face_type::SMILE_OPENED, face_type::SMILE_CLOSED))
      return;

    engine_.get_field().reset();
    SDL_Log("generating_state");
  }

  void generating_state::on_left_field_click(bool is_released, size_t x, size_t y)
  {
    if (change_face_on_click(is_released, face_type::WAITING, face_type::SMILE_CLOSED))
    {
      return;
    }

    auto& field = engine_.get_field();
    field.generate(x, y);
    field.reveal_closed(x, y);

    engine_.set_state<playing_state>();
  }

  void generating_state::on_right_field_click(bool is_released, size_t x, size_t y)
  {
    if (!is_released)
      return;

    auto& field = engine_.get_field();
    field.set_flag(x, y);
  }
}  // namespace mswpr
