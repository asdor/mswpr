#include <vector>
#include <utility>

#include "game.hpp"

namespace mswpr
{

constexpr size_t field_width = 9;
constexpr size_t field_height = 9;

game::game(std::string_view title, size_t xpos, size_t ypos, size_t width, size_t height)
    : is_running_(false), minefield_(field_width, field_height)
{
    const auto window_mode = 0;
    window_.reset(SDL_CreateWindow(title.data(), xpos, ypos, width, height, window_mode));
    if (!window_)
    {
        SDL_Log("Unable to create SDL_window: %s", SDL_GetError());
        return;
    }

    SDL_Log("Window created!\n");

    renderer_.reset(SDL_CreateRenderer(window_.get(), -1, 0), mswpr::sdl_deleter{});
    if (!renderer_)
    {
        SDL_Log("Unable to create SDL_Renderer: %s", SDL_GetError());
        return;
    }

    SDL_SetRenderDrawColor(renderer_.get(), 255, 255, 255, 255);
    SDL_Log("Renderer created!\n");

    texture_manager_.init(renderer_, "../assets/tile.png");
    is_running_ = true;
}

void game::handle_input()
{
    SDL_Event event;
    bool is_released = false;
    int key = -1;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYUP:
            is_released = true;
            [[fallthrough]];
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                is_running_ = false;
                return;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            is_released = true;
            [[fallthrough]];
        case SDL_MOUSEBUTTONDOWN:
            key = event.button.button;
            break;
        default:
            break;
        }
    }

    if (is_released)
    {
        int mouse_x = 0;
        int mouse_y = 0;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        SDL_Log("(%d, %d)", mouse_x, mouse_y);
        if (key == SDL_BUTTON_LEFT)
            minefield_.on_left_click(mouse_x, mouse_y);
        else if (key == SDL_BUTTON_RIGHT)
            minefield_.on_right_click(mouse_x, mouse_y);
    }
}

void game::update()
{
    minefield_.generate();
}

void game::render()
{
    SDL_RenderClear(renderer_.get());

    minefield_.render(texture_manager_);

    SDL_RenderPresent(renderer_.get());
}

} // namespace mswpr
