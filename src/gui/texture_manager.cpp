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

  void texture_manager::init(mswpr::sdl_renderer_t renderer, std::string_view faces_path, std::string_view sprites_path)
  {
    renderer_ = renderer;
    faces_texture_ = load_texture(renderer_, faces_path);
    sprites_texture_ = load_texture(renderer_, sprites_path);

    faces_config_[enum_to<size_t>(face_type::SMILE_CLOSED)] = { 0, 0, 24, 24 };
    faces_config_[enum_to<size_t>(face_type::SMILE_OPENED)] = { 24, 0, 24, 24 };
    faces_config_[enum_to<size_t>(face_type::WAITING)] = { 48, 0, 24, 24 };
    faces_config_[enum_to<size_t>(face_type::BOSS)] = { 72, 0, 24, 24 };
    faces_config_[enum_to<size_t>(face_type::DEAD)] = { 96, 0, 24, 24 };

    sprites_config_[enum_to<size_t>(sprite_type::EMPTY_CLOSED)] = { 0, 0, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::EMPTY_OPENED)] = { 16, 0, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::FLAG)] = { 32, 0, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::QUESTION)] = { 48, 0, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::QUESTION_OPENED)] = { 64, 0, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::BOMB)] = { 80, 0, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::BOMB_RED)] = { 96, 0, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::BOMB_FAILED)] = { 112, 0, 16, 16 };

    sprites_config_[enum_to<size_t>(sprite_type::ONE)] = { 0, 16, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::TWO)] = { 16, 16, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::THREE)] = { 32, 16, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::FOUR)] = { 48, 16, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::FIVE)] = { 64, 16, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::SIX)] = { 80, 16, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::SEVEN)] = { 96, 16, 16, 16 };
    sprites_config_[enum_to<size_t>(sprite_type::EIGHT)] = { 112, 16, 16, 16 };
  }

  void texture_manager::draw(face_type face, SDL_Rect dst)
  {
    const auto index = enum_to<size_t>(face);
    SDL_RenderCopy(renderer_.get(), faces_texture_.get(), &faces_config_[index], &dst);
  }

  void texture_manager::draw(sprite_type sprite, SDL_Rect dst)
  {
    const auto index = enum_to<size_t>(sprite);
    SDL_RenderCopy(renderer_.get(), sprites_texture_.get(), &sprites_config_[index], &dst);
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
