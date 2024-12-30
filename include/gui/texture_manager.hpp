#ifndef MSWP_TEXTURE_MANAGER_HPP
#define MSWP_TEXTURE_MANAGER_HPP

#include <array>
#include <string_view>

#include "sdl_helper.hpp"
#include "core/types.hpp"

#include <SDL.h>

namespace mswpr
{
  class texture_manager
  {
  public:
    texture_manager();

    void init(mswpr::sdl_renderer_t renderer, std::string_view sprite_path);

    void draw(mswpr::face_type i_face, SDL_Rect i_dst);
    void draw(mswpr::sprite_type i_sprite, SDL_Rect i_dst);
    void draw(mswpr::display_digits_type i_digits, SDL_Rect i_dst);

  private:
    static mswpr::sdl_texture_t load_texture(mswpr::sdl_renderer_t renderer, std::string_view path);

    template<mswpr::Enumeration Enum, size_t N>
    void render_sprite(Enum i_index_val, SDL_Rect i_dst_rect, const std::array<SDL_Rect, N>& i_sprites_config)
    {
      const auto index = enum_to<size_t>(i_index_val);
      SDL_RenderCopy(renderer_.get(), sprite_texture_.get(), &i_sprites_config[index], &i_dst_rect);
    }

    std::array<SDL_Rect, mswpr::FACES_COUNT> faces_config_;
    std::array<SDL_Rect, mswpr::SPRITES_COUNT> sprites_config_;

    std::array<SDL_Rect, mswpr::DIPLAY_DIGITS_COUNT> display_digits_config_;

    mswpr::sdl_texture_t sprite_texture_;
    mswpr::sdl_renderer_t renderer_;
  };
}  // namespace mswpr

#endif  // MSWP_TEXTURE_MANAGER_HPP
