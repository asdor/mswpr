#include "states/state_interface.hpp"
#include "game_engine.hpp"


namespace mswpr
{
state_interface::state_interface(game_engine& engine) : engine_(engine) {}

void state_interface::on_left_face_click()
{
}

void state_interface::on_left_field_click(size_t, size_t)
{

}

void state_interface::on_right_field_click(size_t, size_t)
{

}

} // namespace mswpr
