#ifndef MSWPR_STATE_INTERFACE_HPP
#define MSWPR_STATE_INTERFACE_HPP

#include <cstddef>

namespace mswpr
{
class game_engine;

class state_interface
{
public:
    state_interface(game_engine& engine);

    void on_left_face_click();
    void on_left_field_click(size_t x, size_t y);
    void on_right_field_click(size_t x, size_t y);
protected:
    game_engine& engine_;
};
} // namespace mswpr

#endif // MSWPR_STATE_INTERFACE_HPP
