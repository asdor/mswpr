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

void playing_state::on_left_field_click(bool /*is_released*/, size_t x, size_t y)
{
    auto& field = engine_.get_field();
    field(x, y).state = cell_state::OPENED;
}

} // namespace mswpr
