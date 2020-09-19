#ifndef MSWPR_SDL_HELPER_HPP
#define MSWPR_SDL_HELPER_HPP

#include <memory>

#include <SDL.h>

namespace mswpr
{
class sdl_init_t
{
public:
    sdl_init_t();
    bool is_init() const;
    ~sdl_init_t();
};

struct sdl_deleter
{
    void operator()(SDL_Texture* tex) const;
    void operator()(SDL_Window* win) const;
    void operator()(SDL_Renderer* renderer) const;
};

using sdl_texture_t = std::shared_ptr<SDL_Texture>;
using sdl_window_t = std::unique_ptr<SDL_Window, sdl_deleter>;
using sdl_renderer_t = std::shared_ptr<SDL_Renderer>;


} // namespace mswpr

#endif // MSWPR_SDL_HELPER_HPP
