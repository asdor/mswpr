#include "states/state_interface.hpp"
#include "game_engine.hpp"


namespace mswpr
{
state_interface::state_interface(game_engine& engine) : engine_(engine) {}

void state_interface::on_left_face_click(bool /*is_released*/)
{
}

void state_interface::on_left_field_click(bool /*is_released*/, size_t, size_t)
{

}

void state_interface::on_right_field_click(bool is_released, size_t, size_t)
{

}

bool state_interface::change_face_on_click(bool is_released, face_type pressed, face_type released)
{
    if (is_released)
    {
        engine_.set_face(released);
        return false;
    }

    engine_.set_face(pressed);
    return true;
}

} // namespace mswpr
