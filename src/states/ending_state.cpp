#include "game_engine.hpp"
#include "states/ending_state.hpp"
#include "states/generating_state.hpp"

namespace mswpr
{
void ending_state::on_left_face_click()
{
    SDL_Log("ending_state");
    engine_.set_state<generating_state>();
}
} // namespace mswpr
