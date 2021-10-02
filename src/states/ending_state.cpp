#include "game_engine.hpp"
#include "states/ending_state.hpp"
#include "states/generating_state.hpp"

namespace mswpr
{
  ending_state::ending_state(mswpr::game_engine& engine, size_t x, size_t y) : ending_state(engine_)
  {
    engine_.get_field().detonate_bomb(x, y);

    engine_.set_face(face_type::DEAD);
  }

  void ending_state::on_left_face_click(bool is_released)
  {
    if (change_face_on_click(is_released, face_type::SMILE_OPENED, face_type::DEAD))
      return;

    SDL_Log("ending_state");
    engine_.set_state<generating_state>();
  }
}  // namespace mswpr
