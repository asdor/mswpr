#include "gui/sdl_helper.hpp"

#include <SDL.h>
#include <spdlog/spdlog.h>

namespace
{
  constexpr Uint32 INIT_FLAG = SDL_INIT_VIDEO;
}

namespace mswpr
{
  sdl_init_t::sdl_init_t()
  {
    if (SDL_Init(INIT_FLAG) != 0)
      spdlog::error("Unable to initialize SDL: {}", SDL_GetError());
  }

  mswpr::sdl_init_t::~sdl_init_t()
  {
    spdlog::debug("Cleaning up SDL");
    SDL_Quit();
  }

  void sdl_deleter::operator()(SDL_Texture* tex) const
  {
    spdlog::debug("dtor: SDL_Texture");
    SDL_DestroyTexture(tex);
  }

  void sdl_deleter::operator()(SDL_Window* win) const
  {
    spdlog::debug("dtor: SDL_Window");
    SDL_DestroyWindow(win);
  }

  void sdl_deleter::operator()(SDL_Renderer* ren) const
  {
    spdlog::debug("dtor: SDL_Renderer");
    SDL_DestroyRenderer(ren);
  }
}  // namespace mswpr
