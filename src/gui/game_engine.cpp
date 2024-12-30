#include "core/game_config.hpp"
#include "core/states/state_machine.hpp"
#include "gui/game_engine.hpp"

#include <SDL.h>

namespace mswpr
{
  game_engine::game_engine(std::string_view title, size_t xpos, size_t ypos) :

    is_running_(false),
    renderer_(title, xpos, ypos),
    minefield_(cfg::field_width, cfg::field_height, cfg::mines_cnt),
    face_type_(face_type::SMILE_NOT_PRESSED),
    counter_(cfg::mines_cnt),
    state_(minefield_, face_type_, counter_),
    frame_start_ticks_(0)
  {
    is_running_ = true;
    timer_.start(game_timer::now());
  }

  bool game_engine::running() const
  {
    return is_running_;
  }

  void game_engine::handle_input()
  {
    frame_start_ticks_ = SDL_GetTicks();

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
      case SDL_MOUSEMOTION:
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

    if (is_left_btn && renderer_.is_inside_face(mouse_x, mouse_y))
    {
      state_.on_left_face_click(is_released);
    }
    else if (renderer_.is_inside_field(mouse_x, mouse_y))
    {
      const int x = (mouse_x - cfg::board_offset_x) / cfg::cell_width;
      const int y = (mouse_y - cfg::board_offset_y) / cfg::cell_height;
      if (is_left_btn)
      {
        state_.on_left_field_click(is_released, x, y);
      }
      else if (is_right_btn)
      {
        state_.on_right_field_click(is_released, x, y);
      }
    }
  }

  void game_engine::render()
  {
    renderer_.render(minefield_, face_type_, counter_, timer_);
  }

  void game_engine::update()
  {
    // const auto now = std::chrono::steady_clock::now();
    timer_.update(game_timer::now());
  }

  void game_engine::limit_fps()
  {
    const auto frame_ticks = SDL_GetTicks() - frame_start_ticks_;

    if (cfg::frame_delay > frame_ticks)
    {
      SDL_Delay(cfg::frame_delay - frame_ticks);
    }
  }

}  // namespace mswpr
