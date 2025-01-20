#ifndef MSWP_TEXTURE_MANAGER_HPP
#define MSWP_TEXTURE_MANAGER_HPP

#include <array>
#include <string_view>

#include "core/types.hpp"
#include "gui/sdl_helper.hpp"

#include <SDL.h>

namespace mswpr
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
  class texture_manager
  {
  public:
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    texture_manager() = default;

    void init(mswpr::sdl_renderer_t renderer, std::string_view sprite_path);

    void draw(mswpr::face_type i_face, SDL_Rect i_dst);
    void draw(mswpr::sprite_type i_sprite, SDL_Rect i_dst);
    void draw(mswpr::display_digits_type i_digits, SDL_Rect i_dst);
    void draw(mswpr::border_type i_border, SDL_Rect i_dst);

  private:
    static mswpr::sdl_texture_t load_texture(mswpr::sdl_renderer_t renderer, std::string_view path);

    template<mswpr::Enumeration Enum, size_t N>
    void render_sprite(Enum i_index_val, SDL_Rect i_dst_rect, const std::array<SDL_Rect, N>& i_sprites_config)
    {
      const auto index = enum_to<size_t>(i_index_val);
      SDL_RenderCopy(d_renderer.get(), d_sprite_texture.get(), &i_sprites_config[index], &i_dst_rect);
    }

    std::array<SDL_Rect, mswpr::FACES_COUNT> d_faces_config;
    std::array<SDL_Rect, mswpr::SPRITES_COUNT> d_sprites_config;
    std::array<SDL_Rect, mswpr::DIPLAY_DIGITS_COUNT> d_display_digits_config;
    std::array<SDL_Rect, mswpr::BORDER_TYPE_COUNT> d_border_config;

    mswpr::sdl_texture_t d_sprite_texture;
    mswpr::sdl_renderer_t d_renderer;
  };
}  // namespace mswpr

#endif  // MSWP_TEXTURE_MANAGER_HPP
