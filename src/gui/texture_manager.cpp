#include "gui/texture_manager.hpp"
#include "core/types.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include <stdexcept>

namespace mswpr
{
  texture_manager::texture_manager()
  {
  }

  void texture_manager::init(mswpr::sdl_renderer_t renderer, std::string_view sprite_path)
  {
    renderer_ = renderer;
    sprite_texture_ = load_texture(renderer_, sprite_path);

    constexpr int FACE_SIZE = 26;
    faces_config_[enum_to<size_t>(face_type::SMILE_NOT_PRESSED)] = { 0, 55, FACE_SIZE, FACE_SIZE };
    faces_config_[enum_to<size_t>(face_type::WAITING)] = { 27, 55, FACE_SIZE, FACE_SIZE };
    faces_config_[enum_to<size_t>(face_type::DEAD)] = { 2 * 27, 55, FACE_SIZE, FACE_SIZE };
    faces_config_[enum_to<size_t>(face_type::BOSS)] = { 3 * 27, 55, FACE_SIZE, FACE_SIZE };
    faces_config_[enum_to<size_t>(face_type::SMILE_PRESSED)] = { 4 * 27, 55, FACE_SIZE, FACE_SIZE };

    constexpr int CELL_SIZE = 16;
    sprites_config_[enum_to<size_t>(sprite_type::EMPTY_OPENED)] = { 0, 0, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::ONE)] = { 16, 0, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::TWO)] = { 32, 0, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::THREE)] = { 48, 0, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::FOUR)] = { 64, 0, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::FIVE)] = { 80, 0, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::SIX)] = { 96, 0, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::SEVEN)] = { 112, 0, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::EIGHT)] = { 128, 0, CELL_SIZE, CELL_SIZE };

    sprites_config_[enum_to<size_t>(sprite_type::EMPTY_CLOSED)] = { 0, 16, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::BOMB)] = { 32, 16, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::FLAG)] = { 48, 16, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::BOMB_FAILED)] = { 64, 16, CELL_SIZE, CELL_SIZE };
    sprites_config_[enum_to<size_t>(sprite_type::BOMB_RED)] = { 80, 16, CELL_SIZE, CELL_SIZE };

    static constexpr int DIGIT_WIDTH = 11;
    static constexpr int DIGIT_HEIGHT = 21;
    display_digits_config_[enum_to<size_t>(display_digits_type::EMPTY_DISPLAY)] = { 28, 82, 41, 25 };

    for (size_t i = enum_to<size_t>(display_digits_type::ZERO); i <= enum_to<size_t>(display_digits_type::MINUS); ++i)
    {
      const size_t rect_x = (DIGIT_WIDTH + 1) * (i - enum_to<size_t>(display_digits_type::ZERO));
      display_digits_config_[i] = { static_cast<int>(rect_x), 33, DIGIT_WIDTH, DIGIT_HEIGHT };
    }
  }

  void texture_manager::draw(face_type face, SDL_Rect dst)
  {
    const auto index = enum_to<size_t>(face);
    SDL_RenderCopy(renderer_.get(), sprite_texture_.get(), &faces_config_[index], &dst);
  }

  void texture_manager::draw(sprite_type sprite, SDL_Rect dst)
  {
    const auto index = enum_to<size_t>(sprite);
    SDL_RenderCopy(renderer_.get(), sprite_texture_.get(), &sprites_config_[index], &dst);
  }

  void texture_manager::draw(mswpr::display_digits_type digits, SDL_Rect dst)
  {
    const auto index = enum_to<size_t>(digits);
    SDL_RenderCopy(renderer_.get(), sprite_texture_.get(), &display_digits_config_[index], &dst);
  }

  mswpr::sdl_texture_t texture_manager::load_texture(mswpr::sdl_renderer_t renderer, std::string_view path)
  {
    mswpr::sdl_texture_t texture(IMG_LoadTexture(renderer.get(), path.data()), mswpr::sdl_deleter{});
    if (!texture)
    {
      const std::string err = "Failed to open " + std::string(path);
      SDL_Log("%s, error: %s\n", err.c_str(), IMG_GetError());
      throw std::runtime_error(err.c_str());
    }

    return texture;
  }

}  // namespace mswpr
