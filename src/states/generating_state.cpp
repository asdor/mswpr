#include "game_engine.hpp"
#include "states/generating_state.hpp"
#include "states/playing_state.hpp"

namespace mswpr
{
void generating_state::on_left_face_click(bool is_released)
{
    if (change_face_on_click(is_released, face_type::SMILE_OPENED, face_type::SMILE_CLOSED))
        return;

    SDL_Log("generating_state");
    engine_.set_state<playing_state>();
}

void generating_state::on_left_field_click(bool is_released, size_t, size_t)
{
    if (change_face_on_click(is_released, face_type::WAITING, face_type::SMILE_CLOSED))
    {
        return;
    }
}
} // namespace mswpr
