#include "game_engine.hpp"
#include "states/generating_state.hpp"
#include "states/playing_state.hpp"

namespace mswpr
{
void generating_state::on_left_face_click()
{
    SDL_Log("generating_state");
    engine_.set_face(face_type::SMILE_CLOSED);
    engine_.set_state<playing_state>();
}

} // namespace mswpr
