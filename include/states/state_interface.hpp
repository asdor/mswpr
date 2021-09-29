#ifndef MSWPR_STATE_INTERFACE_HPP
#define MSWPR_STATE_INTERFACE_HPP

#include <cstddef>

namespace mswpr
{
class game_engine;

enum class face_type : size_t;

class state_interface
{
public:
    state_interface(game_engine& engine);

    void on_left_face_click(bool is_released);
    void on_left_field_click(bool is_released, size_t x, size_t y);
    void on_right_field_click(bool is_released, size_t x, size_t y);
protected:
    bool change_face_on_click(bool is_released, face_type pressed, face_type released);

    game_engine& engine_;
};
} // namespace mswpr

#endif // MSWPR_STATE_INTERFACE_HPP
