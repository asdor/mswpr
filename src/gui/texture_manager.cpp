#include "gui/texture_manager.hpp"
#include "core/types.hpp"
#include "gui/logger.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

#include <format>
#include <stdexcept>
#include <string>

namespace mswpr
{
  void texture_manager::init(mswpr::sdl_renderer_t renderer, std::string_view sprite_path)
  {
    d_renderer = renderer;
    d_sprite_texture = load_texture(d_renderer, sprite_path);

    constexpr int FACE_SIZE = 26;
    d_faces_config[enum_to<size_t>(face_type::SMILE_NOT_PRESSED)] = { 0, 55, FACE_SIZE, FACE_SIZE };
    d_faces_config[enum_to<size_t>(face_type::WAITING)] = { 27, 55, FACE_SIZE, FACE_SIZE };
    d_faces_config[enum_to<size_t>(face_type::DEAD)] = { 2 * 27, 55, FACE_SIZE, FACE_SIZE };
    d_faces_config[enum_to<size_t>(face_type::BOSS)] = { 3 * 27, 55, FACE_SIZE, FACE_SIZE };
    d_faces_config[enum_to<size_t>(face_type::SMILE_PRESSED)] = { 4 * 27, 55, FACE_SIZE, FACE_SIZE };

    constexpr int CELL_SIZE = 16;
    d_sprites_config[enum_to<size_t>(sprite_type::EMPTY_OPENED)] = { 0, 0, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::ONE)] = { 16, 0, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::TWO)] = { 32, 0, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::THREE)] = { 48, 0, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::FOUR)] = { 64, 0, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::FIVE)] = { 80, 0, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::SIX)] = { 96, 0, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::SEVEN)] = { 112, 0, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::EIGHT)] = { 128, 0, CELL_SIZE, CELL_SIZE };

    d_sprites_config[enum_to<size_t>(sprite_type::EMPTY_CLOSED)] = { 0, 16, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::BOMB)] = { 32, 16, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::FLAG)] = { 48, 16, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::BOMB_FAILED)] = { 64, 16, CELL_SIZE, CELL_SIZE };
    d_sprites_config[enum_to<size_t>(sprite_type::BOMB_RED)] = { 80, 16, CELL_SIZE, CELL_SIZE };

    static constexpr int DIGIT_WIDTH = 11;
    static constexpr int DIGIT_HEIGHT = 21;
    d_display_digits_config[enum_to<size_t>(display_digits_type::EMPTY_DISPLAY)] = { 28, 82, 41, 25 };

    for (auto i = enum_to<size_t>(display_digits_type::ZERO); i <= enum_to<size_t>(display_digits_type::MINUS); ++i)
    {
      const size_t rect_x = (DIGIT_WIDTH + 1) * (i - enum_to<size_t>(display_digits_type::ZERO));
      d_display_digits_config[i] = { static_cast<int>(rect_x), 33, DIGIT_WIDTH, DIGIT_HEIGHT };
    }

    static constexpr int BORDER_WIDTH = 12;
    static constexpr int BORDER_TOP_HEIGHT = 11;
    static constexpr int BORDER_BOTTOM_HEIGHT = 12;
    static constexpr int BORDER_STRIP_SIZE = 1;
    d_border_config[enum_to<size_t>(border_type::TOP_LEFT)] = { 0, 82, BORDER_WIDTH, BORDER_TOP_HEIGHT };
    d_border_config[enum_to<size_t>(border_type::TOP_RIGHT)] = { 15, 82, BORDER_WIDTH, BORDER_TOP_HEIGHT };
    d_border_config[enum_to<size_t>(border_type::HUD_TOP)] = { 13, 82, BORDER_STRIP_SIZE, BORDER_TOP_HEIGHT };

    d_border_config[enum_to<size_t>(border_type::HUD_LEFT)] = { 0, 94, BORDER_WIDTH, BORDER_STRIP_SIZE };
    d_border_config[enum_to<size_t>(border_type::HUD_RIGHT)] = { 15, 94, BORDER_WIDTH, BORDER_STRIP_SIZE };
    d_border_config[enum_to<size_t>(border_type::DIVIDER_LEFT)] = { 0, 96, BORDER_WIDTH, BORDER_TOP_HEIGHT };
    d_border_config[enum_to<size_t>(border_type::DIVIDER_MIDDLE)] = { 13, 96, BORDER_STRIP_SIZE, BORDER_TOP_HEIGHT };
    d_border_config[enum_to<size_t>(border_type::DIVIDER_RIGHT)] = { 15, 96, BORDER_WIDTH, BORDER_TOP_HEIGHT };
    d_border_config[enum_to<size_t>(border_type::FIELD_LEFT)] = { 0, 108, BORDER_WIDTH, BORDER_STRIP_SIZE };
    d_border_config[enum_to<size_t>(border_type::FIELD_RIGHT)] = { 15, 108, BORDER_WIDTH, BORDER_STRIP_SIZE };
    d_border_config[enum_to<size_t>(border_type::BOTTOM_LEFT)] = { 0, 110, BORDER_WIDTH, BORDER_BOTTOM_HEIGHT };
    d_border_config[enum_to<size_t>(border_type::BOTTOM_RIGHT)] = { 15, 110, BORDER_WIDTH, BORDER_BOTTOM_HEIGHT };
    d_border_config[enum_to<size_t>(border_type::FIELD_BOTTOM)] = { 13, 110, BORDER_STRIP_SIZE, BORDER_BOTTOM_HEIGHT };
  }

  void texture_manager::draw(face_type i_face, SDL_Rect i_dst)
  {
    render_sprite(i_face, i_dst, d_faces_config);
  }

  void texture_manager::draw(sprite_type i_sprite, SDL_Rect i_dst)
  {
    render_sprite(i_sprite, i_dst, d_sprites_config);
  }

  void texture_manager::draw(mswpr::display_digits_type i_digits, SDL_Rect i_dst)
  {
    render_sprite(i_digits, i_dst, d_display_digits_config);
  }

  void texture_manager::draw(mswpr::border_type i_border, SDL_Rect i_dst)
  {
    render_sprite(i_border, i_dst, d_border_config);
  }

  mswpr::sdl_texture_t texture_manager::load_texture(mswpr::sdl_renderer_t renderer, std::string_view path)
  {
    mswpr::sdl_texture_t texture(IMG_LoadTexture(renderer.get(), path.data()), mswpr::sdl_deleter{});
    if (!texture)
    {
      const auto err_msg = std::format("Failed to open {}, error: {}.", path, IMG_GetError());
      spdlog::get("engine")->error(err_msg);
      throw std::runtime_error(err_msg);
    }

    return texture;
  }

}  // namespace mswpr
