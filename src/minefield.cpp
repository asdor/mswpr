#include "minefield.hpp"
#include "texture_manager.hpp"
#include "types.hpp"

namespace
{
constexpr minesweeper::sprite_type to_sprite(minesweeper::cell_value value)
{
    const size_t index = minesweeper::enum_to<size_t>(value);
    constexpr size_t empty_index = minesweeper::enum_to<size_t>(minesweeper::sprite_type::EMPTY_OPENED);
    return minesweeper::to_enum<minesweeper::sprite_type>(empty_index + index);
}

} // namespace

namespace minesweeper
{
minefield::minefield(size_t width, size_t height)
    : width_(width), height_(height), field_(width_ * height_, {cell_value::EMPTY, cell_state::CLOSED})
{
}

void minefield::handle_input()
{
}

void minefield::update()
{
}

void minefield::render(texture_manager& manager)
{
    for (size_t i = 0; i < height_; ++i)
    {
        for (size_t j = 0; j < width_; ++j)
        {
            const auto item = field_[i * width_ + j];
            const SDL_Rect dst_rect = {int(j * 32), int(i * 32), 32, 32};

            sprite_type sprite = sprite_type::EMPTY_CLOSED;

            switch (item.state)
            {
            case cell_state::CLOSED:
                sprite = sprite_type::EMPTY_CLOSED;
                break;
            case cell_state::FLAGGED:
                sprite = sprite_type::FLAG;
                break;
            case cell_state::OPENED:
                sprite = to_sprite(item.value);
                break;
            default:
                break;
            }

            manager.draw(sprite, dst_rect);
        }
    }
}

void minefield::generate()
{
    auto& elem = field_[5 * width_ + 3];
    elem.state = cell_state::OPENED;
    field_[5 * width_ + 4].state = cell_state::OPENED;
    for (size_t i = 0; i < 9; ++i)
    {
        field_[4 * width_ + i].value = to_enum<cell_value>(i);
    }

    field_[4 * width_ + 3].state = cell_state::OPENED;
    field_[4 * width_ + 4].state = cell_state::OPENED;
    field_.back().state = cell_state::OPENED;
}

void minefield::on_left_click(size_t mouse_x, size_t mouse_y)
{
    const size_t x = mouse_x / 32;
    const size_t y = mouse_y / 32;
    SDL_Log("Left mouse click at (%ld, %ld)", x, y);
    if (x >= width_ || y >= height_)
        return;

    auto& elem = field_[y * width_ + x];
    if (elem.state == cell_state::CLOSED)
        elem.state = cell_state::OPENED;
}

void minefield::on_right_click(size_t mouse_x, size_t mouse_y)
{
    const size_t x = mouse_x / 32;
    const size_t y = mouse_y / 32;
    SDL_Log("Right mouse click at (%ld, %ld)", x, y);
    if (x >= width_ || y >= height_)
        return;

    auto& elem = field_[y * width_ + x];
    if (elem.state == cell_state::CLOSED)
        elem.state = cell_state::FLAGGED;
    else if (elem.state == cell_state::FLAGGED)
        elem.state = cell_state::CLOSED;
}

} // namespace minesweeper
