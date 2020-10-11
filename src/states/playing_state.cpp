#include "game_engine.hpp"
#include "states/ending_state.hpp"
#include "states/playing_state.hpp"

namespace mswpr
{
void playing_state::on_left_face_click()
{
    SDL_Log("playing_state");
    engine_.set_face(face_type::BOSS);
    engine_.set_state<ending_state>();
}

} // namespace mswpr
