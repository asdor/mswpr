#ifndef MSWPR_SDL_HELPER_HPP
#define MSWPR_SDL_HELPER_HPP

#include <memory>

#include <SDL3/SDL.h>

namespace mswpr
{
  class sdl_init_t
  {
  public:
    sdl_init_t();
    sdl_init_t(const sdl_init_t&) = delete;
    sdl_init_t(sdl_init_t&&) = delete;
    sdl_init_t& operator=(const sdl_init_t&) = delete;
    sdl_init_t& operator=(sdl_init_t&&) = delete;
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

}  // namespace mswpr

#endif  // MSWPR_SDL_HELPER_HPP
