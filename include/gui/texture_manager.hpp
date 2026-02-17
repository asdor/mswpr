#ifndef MSWP_TEXTURE_MANAGER_HPP
#define MSWP_TEXTURE_MANAGER_HPP

#include <array>
#include <string_view>

#include "core/types.hpp"
#include "gui/sdl_helper.hpp"

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>

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

    static constexpr SDL_FRect rect_to_float_rect(SDL_Rect i_rect) noexcept
    {
      return { static_cast<float>(i_rect.x),
               static_cast<float>(i_rect.y),
               static_cast<float>(i_rect.w),
               static_cast<float>(i_rect.h) };
    }

    template<mswpr::Enumeration Enum, size_t N>
    void render_sprite(Enum i_index_val, SDL_Rect i_dst_rect, const std::array<SDL_Rect, N>& i_sprites_config)
    {
      const auto index = enum_to<size_t>(i_index_val);
      const auto src_rect = rect_to_float_rect(i_sprites_config[index]);
      const auto dst_rect = rect_to_float_rect(i_dst_rect);
      if (!SDL_RenderTexture(d_renderer.get(), d_sprite_texture.get(), &src_rect, &dst_rect))
      {
        const auto err_msg = std::format("Failed to render texture, error: {}.", SDL_GetError());
        spdlog::get("engine")->error(err_msg);
        throw std::runtime_error(err_msg);
      }
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
