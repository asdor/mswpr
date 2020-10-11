#include <SDL.h>

#include "game_config.hpp"
#include "game_engine.hpp"

namespace mswpr
{
game_engine::game_engine(std::string_view title, size_t xpos, size_t ypos, size_t width, size_t height)
    : is_running_(false), minefield_(cfg::field_width, cfg::field_height, cfg::mines_cnt),
      face_type_(face_type::SMILE_CLOSED),
      state_(std::in_place_type<generating_state>, *this)
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

    SDL_SetRenderDrawColor(renderer_.get(), 190, 190, 190, 0);
    SDL_Log("Renderer created!\n");

    texture_manager_.init(renderer_, "../assets/faces.png", "../assets/tile.png");
    is_running_ = true;
}

bool game_engine::running() const
{
    return is_running_;
}

void game_engine::handle_input()
{
    bool is_released = false;
    [[maybe_unused]] int key = -1;

    SDL_Event event;
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
            // SDL_Log("SDL_MOUSEBUTTONUP");
            is_released = true;
            [[fallthrough]];
        case SDL_MOUSEBUTTONDOWN:
            // SDL_Log("SDL_MOUSEBUTTONDOWN");
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
        // SDL_Log("(%d, %d)", mouse_x, mouse_y);
        if (key == SDL_BUTTON_LEFT)
        {
            std::visit([](auto& st) {
                st.on_left_face_click();
            }, state_);
        }

        // if (key == SDL_BUTTON_LEFT)
        //     minefield_.on_left_click(mouse_x, mouse_y);
        // else if (key == SDL_BUTTON_RIGHT)
        //     minefield_.on_right_click(mouse_x, mouse_y);
    }
}

void game_engine::render()
{
    SDL_RenderClear(renderer_.get());

    minefield_.render(texture_manager_);

    const int face_x = cfg::field_width * cfg::cell_width / 2 - cfg::face_width / 2 + cfg::board_offset_x;
    const SDL_Rect face_dst = {face_x, 0, cfg::face_width, cfg::face_height};
    texture_manager_.draw(face_type_, face_dst);

    SDL_RenderPresent(renderer_.get());
}

void game_engine::update()
{

}

void game_engine::set_face(face_type face)
{
    face_type_ = face;
}

} // namespace mswpr
