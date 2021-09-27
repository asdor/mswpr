#include <algorithm>
#include <random>
#include <stack>

#include "game_engine.hpp"
#include "states/generating_state.hpp"
#include "states/playing_state.hpp"

namespace mswpr
{
generating_state::generating_state(game_engine& engine) : state_interface(engine)
{
    // engine_.get_field().reset();
}

void generating_state::on_left_face_click(bool is_released)
{
    if (change_face_on_click(is_released, face_type::SMILE_OPENED, face_type::SMILE_CLOSED))
        return;

    SDL_Log("generating_state");
    engine_.set_state<playing_state>();
}

void generating_state::on_left_field_click(bool is_released, size_t elem_x, size_t elem_y)
{
    auto& field = engine_.get_field();
    field.reset();
    // if (!is_released)
    //     field(x, y).state = cell_state::OPENED;
    // else
    //     field(x, y).state = cell_state::CLOSED;

    if (change_face_on_click(is_released, face_type::WAITING, face_type::SMILE_CLOSED))
        return;

    // auto bomb = [this](size_t x, size_t y) {
    //     field_[y * width_ + x].value = cell_value::BOMB;
    // };
    // bomb(3, 0);
    // bomb(7, 0);
    // bomb(2, 1);
    // bomb(3, 1);
    // bomb(6, 1);
    // bomb(0, 2);
    // bomb(6, 3);
    // bomb(7, 4);
    // bomb(4, 6);
    // bomb(5, 6);

    const size_t width = field.width_;
    const size_t height = field.height_;

    std::vector<size_t> coords(width * height, 0);

    std::iota(coords.begin(), coords.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(coords.begin(), coords.end(), g);

    for (size_t i = 0; i < field.bombs_cnt_; ++i)
    {
        const size_t coord = coords[i];
        const size_t x = coord / width;
        const size_t y = coord % width;
        field(x, y).value = cell_value::BOMB;
        SDL_Log("Bomb at %ld, %ld", x, y);
    }

    static constexpr std::array<int, 8> dir_x = {-1, 0, 1, -1, 1, -1, 0, 1};
    static constexpr std::array<int, 8> dir_y = {-1, -1, -1, 0, 0, 1, 1, 1};

    const int width_i = static_cast<int>(width);
    const int height_i = static_cast<int>(height);
    for (int y = 0; y < width_i; ++y)
    {
        for (int x = 0; x < height_i; ++x)
        {
            if (field(x, y).value == cell_value::BOMB)
                continue;

            size_t cnt = 0;
            for (size_t i = 0; i < dir_x.size(); ++i)
            {
                int i_x = x - dir_x[i];
                int i_y = y - dir_y[i];
                if (i_x >= 0 && i_x < width_i && i_y >= 0 && i_y < height_i && field(i_x, i_y).is_bomb())
                    ++cnt;
            }

            field(x, y).value = to_enum<cell_value>(cnt);
        }
    }

    field.reveal_cell(elem_x, elem_y);
    engine_.set_state<playing_state>();
}
} // namespace mswpr
