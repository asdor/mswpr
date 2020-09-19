#ifndef MSWP_TEXTURE_MANAGER_HPP
#define MSWP_TEXTURE_MANAGER_HPP

#include <array>
#include <string_view>

#include "sdl_helper.hpp"
#include "types.hpp"

#include <SDL.h>

namespace minesweeper
{
class texture_manager
{
public:
    texture_manager();
    void init(minesweeper::sdl_renderer_t renderer, std::string_view texture_path);
    void draw(minesweeper::sprite_type, SDL_Rect dst);

private:
    static minesweeper::sdl_texture_t load_texture(minesweeper::sdl_renderer_t renderer, std::string_view path);

    std::array<SDL_Rect, minesweeper::SPRITES_COUNT> sprites_config_;
    minesweeper::sdl_texture_t texture_;
    minesweeper::sdl_renderer_t renderer_;
};
} // namespace minesweeper

#endif // MSWP_TEXTURE_MANAGER_HPP
