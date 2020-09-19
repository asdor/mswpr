#include "minefield.hpp"
#include "texture_manager.hpp"

namespace minesweeper
{
minefield::minefield(size_t width, size_t height)
    : width_(width), height_(height), field_(width_ * height_, {cell_state::EMPTY, false, false})
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
            if (!item.is_pressed)
            {
                manager.draw(sprite_type::EMPTY, dst_rect);
                continue;
            }
            else if (item.is_flagged)
            {
                manager.draw(sprite_type::FLAG, dst_rect);
                continue;
            }

            sprite_type sprite = sprite_type::EMPTY;
            switch (item.state)
            {
            case cell_state::EMPTY:
                sprite = sprite_type::EMPTY_PRESSED;
                break;
            case cell_state::ONE:
                sprite = sprite_type::ONE;
                break;
            case cell_state::TWO:
                sprite = sprite_type::TWO;
                break;
            case cell_state::THREE:
                sprite = sprite_type::THREE;
                break;
            case cell_state::FOUR:
                sprite = sprite_type::FOUR;
                break;
            case cell_state::FIVE:
                sprite = sprite_type::FIVE;
                break;
            case cell_state::SIX:
                sprite = sprite_type::SIX;
                break;
            case cell_state::SEVEN:
                sprite = sprite_type::SEVEN;
                break;
            case cell_state::EIGHT:
                sprite = sprite_type::EIGHT;
                break;
            case cell_state::BOMB:
                sprite = sprite_type::BOMB;
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
    elem.is_pressed = true;
    field_[5 * width_ + 4].is_pressed = true;
    field_[4 * width_ + 3].is_pressed = true;
    field_[4 * width_ + 4].is_pressed = true;
    field_.back().is_pressed = true;
}

void minefield::on_left_click(size_t mouse_x, size_t mouse_y)
{
    const size_t x = mouse_x / 32;
    const size_t y = mouse_y / 32;
    SDL_Log("Left mouse click at (%ld, %ld)", x, y);
    auto& elem = field_[y * width_ + x];
    if (!elem.is_flagged)
        field_[y * width_ + x].is_pressed = true;
}

void minefield::on_right_click(size_t mouse_x, size_t mouse_y)
{
    const size_t x = mouse_x / 32;
    const size_t y = mouse_y / 32;
    SDL_Log("Right mouse click at (%ld, %ld)", x, y);
    auto& elem = field_[y * width_ + x];
    if (!elem.is_pressed)
        elem.is_flagged = !elem.is_flagged;
}


} // namespace minesweeper
