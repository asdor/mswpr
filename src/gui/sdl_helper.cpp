#include "gui/sdl_helper.hpp"
#include "gui/logger.hpp"

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
      get_mswpr_logger()->error("Unable to initialize SDL: {}", SDL_GetError());
  }

  mswpr::sdl_init_t::~sdl_init_t()
  {
    get_mswpr_logger()->debug("Cleaning up SDL");
    SDL_Quit();
  }

  void sdl_deleter::operator()(SDL_Texture* tex) const
  {
    get_mswpr_logger()->debug("dtor: SDL_Texture");
    SDL_DestroyTexture(tex);
  }

  void sdl_deleter::operator()(SDL_Window* win) const
  {
    get_mswpr_logger()->debug("dtor: SDL_Window");
    SDL_DestroyWindow(win);
  }

  void sdl_deleter::operator()(SDL_Renderer* ren) const
  {
    get_mswpr_logger()->debug("dtor: SDL_Renderer");
    SDL_DestroyRenderer(ren);
  }
}  // namespace mswpr
