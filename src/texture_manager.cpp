#include "texture_manager.hpp"

#include <SDL.h>
#include <SDL_image.h>

namespace
{
constexpr size_t to_index(mswpr::sprite_type type)
{
    return static_cast<size_t>(type);
}
} // namespace

namespace mswpr
{
texture_manager::texture_manager()
{
}

void texture_manager::init(mswpr::sdl_renderer_t renderer, std::string_view texture_path)
{
    renderer_ = renderer;
    texture_ = load_texture(renderer_, texture_path);
    sprites_config_[to_index(sprite_type::EMPTY_CLOSED)] = {0, 0, 16, 16};
    sprites_config_[to_index(sprite_type::EMPTY_OPENED)] = {16, 0, 16, 16};
    sprites_config_[to_index(sprite_type::FLAG)] = {32, 0, 16, 16};
    sprites_config_[to_index(sprite_type::QUESTION)] = {48, 0, 16, 16};
    sprites_config_[to_index(sprite_type::QUESTION_OPENED)] = {64, 0, 16, 16};
    sprites_config_[to_index(sprite_type::BOMB)] = {80, 0, 16, 16};
    sprites_config_[to_index(sprite_type::BOMB_RED)] = {96, 0, 16, 16};
    sprites_config_[to_index(sprite_type::BOMB_FAILED)] = {112, 0, 16, 16};

    sprites_config_[to_index(sprite_type::ONE)] = {0, 16, 16, 16};
    sprites_config_[to_index(sprite_type::TWO)] = {16, 16, 16, 16};
    sprites_config_[to_index(sprite_type::THREE)] = {32, 16, 16, 16};
    sprites_config_[to_index(sprite_type::FOUR)] = {48, 16, 16, 16};
    sprites_config_[to_index(sprite_type::FIVE)] = {64, 16, 16, 16};
    sprites_config_[to_index(sprite_type::SIX)] = {80, 16, 16, 16};
    sprites_config_[to_index(sprite_type::SEVEN)] = {96, 16, 16, 16};
    sprites_config_[to_index(sprite_type::EIGHT)] = {112, 16, 16, 16};
}

void texture_manager::draw(sprite_type sprite, SDL_Rect dst)
{
    const auto index = to_index(sprite);
    SDL_RenderCopy(renderer_.get(), texture_.get(), &sprites_config_[index], &dst);
}

mswpr::sdl_texture_t texture_manager::load_texture(mswpr::sdl_renderer_t renderer, std::string_view path)
{
    mswpr::sdl_texture_t texture(IMG_LoadTexture(renderer.get(), path.data()), mswpr::sdl_deleter{});
    if (!texture)
    {
        const std::string err = "Failed to open " + std::string(path);
        throw std::runtime_error(err.c_str());
    }

    return texture;
}

} // namespace mswpr
