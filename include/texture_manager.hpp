#ifndef MSWP_TEXTURE_MANAGER_HPP
#define MSWP_TEXTURE_MANAGER_HPP

#include <array>
#include <string_view>

#include "sdl_helper.hpp"
#include "types.hpp"

#include <SDL.h>

namespace mswpr
{
class texture_manager
{
public:
    texture_manager();

    void init(mswpr::sdl_renderer_t renderer, std::string_view faces_path, std::string_view sprites_path);

    void draw(mswpr::face_type face, SDL_Rect dst);
    void draw(mswpr::sprite_type sprite, SDL_Rect dst);

private:
    static mswpr::sdl_texture_t load_texture(mswpr::sdl_renderer_t renderer, std::string_view path);

    std::array<SDL_Rect, mswpr::FACES_COUNT> faces_config_;
    std::array<SDL_Rect, mswpr::SPRITES_COUNT> sprites_config_;

    mswpr::sdl_texture_t faces_texture_;
    mswpr::sdl_texture_t sprites_texture_;
    mswpr::sdl_renderer_t renderer_;
};
} // namespace mswpr

#endif // MSWP_TEXTURE_MANAGER_HPP
