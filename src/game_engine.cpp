#include <SDL.h>

#include "game_config.hpp"
#include "game_engine.hpp"

namespace
{
bool is_inside(const SDL_Rect& rect, int x, int y)
{
    return x >= rect.x &&
           x < rect.x + rect.w &&
           y >= rect.y &&
           y < rect.y + rect.h;
}

} // namespace

namespace mswpr
{
game_engine::game_engine(std::string_view title, size_t xpos, size_t ypos)
    : face_rect_(), field_rect_(), is_running_(false), minefield_(cfg::field_width, cfg::field_height, cfg::mines_cnt),
      face_type_(face_type::SMILE_CLOSED),
      state_(std::in_place_type<generating_state>, *this)
{
    const auto window_mode = 0;
    const size_t window_width = 2 * cfg::board_offset_x + cfg::cell_width * cfg::field_width;
    const size_t window_height = cfg::board_offset_y + cfg::board_offset_x + cfg::cell_height * cfg::field_height;

    window_.reset(SDL_CreateWindow(title.data(), xpos, ypos, window_width, window_height, window_mode));
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

    texture_manager_.init(renderer_, "assets/faces.png", "assets/tile.png");

    const int face_x = cfg::field_width * cfg::cell_width / 2 - cfg::face_width / 2 + cfg::board_offset_x;
    face_rect_ = {face_x, 0, cfg::face_width, cfg::face_height};

    field_rect_ = {cfg::board_offset_x, cfg::board_offset_y, cfg::cell_width * cfg::field_width, cfg::cell_height * cfg::field_height};

    is_running_ = true;
}

bool game_engine::running() const
{
    return is_running_;
}

void game_engine::handle_input()
{
    bool is_released = false;
    int key = -1;

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
            is_released = true;
            [[fallthrough]];
        case SDL_MOUSEBUTTONDOWN:
            key = event.button.button;
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                is_running_ = false;
                return;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    process_click(is_released, key);
}

void game_engine::process_click(bool is_released, int key)
{
    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    const bool is_left_btn = key == SDL_BUTTON_LEFT;
    const bool is_right_btn = key == SDL_BUTTON_RIGHT;
    if (!(is_left_btn || is_right_btn))
        return;
 
    if (is_left_btn && is_inside(face_rect_, mouse_x, mouse_y))
    {
        std::visit([is_released](auto& st) {
            st.on_left_face_click(is_released);
        }, state_);
    }
    else if (is_inside(field_rect_, mouse_x, mouse_y))
    {
        const int x = (mouse_x - cfg::board_offset_x) / cfg::cell_width;
        const int y = (mouse_y - cfg::board_offset_y) / cfg::cell_height;
        std::visit([is_released, x, y, is_left_btn, is_right_btn](auto& st) {
            if (is_left_btn)
            {
                st.on_left_field_click(is_released, x, y);
            }
            else if (is_right_btn)
            {
                st.on_right_field_click(is_released, x, y);
            }
        }, state_);
    }
}

void game_engine::render()
{
    SDL_RenderClear(renderer_.get());

    minefield_.render(texture_manager_);

    texture_manager_.draw(face_type_, face_rect_);

    SDL_RenderPresent(renderer_.get());
}

void game_engine::update()
{

}

void game_engine::set_face(face_type face)
{
    face_type_ = face;
}

mswpr::minefield& game_engine::get_field()
{
    return minefield_;
}

const mswpr::minefield& game_engine::get_field() const
{
    return minefield_;
}
} // namespace mswpr
