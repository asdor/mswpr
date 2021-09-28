#include "sdl_helper.hpp"

#include <SDL.h>

namespace mswpr
{
// class sdl_init

sdl_init_t::sdl_init_t()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
}

bool sdl_init_t::is_init() const
{
    return SDL_WasInit(SDL_INIT_EVERYTHING);
}

mswpr::sdl_init_t::~sdl_init_t()
{
    SDL_Log("Cleaning up SDL");
    SDL_Quit();
}

// struct sdl_deleter

void sdl_deleter::operator()(SDL_Texture* tex) const
{
    SDL_Log("dtor: SDL_Texture\n");
    SDL_DestroyTexture(tex);
}

void sdl_deleter::operator()(SDL_Window* win) const
{
    SDL_Log("dtor: SDL_Window\n");
    SDL_DestroyWindow(win);
}

void sdl_deleter::operator()(SDL_Renderer* ren) const
{
    SDL_Log("dtor: SDL_Renderer\n");
    SDL_DestroyRenderer(ren);
}
} // namespace mswpr
