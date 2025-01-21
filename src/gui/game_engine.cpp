#include "gui/game_engine.hpp"
#include "core/game_config.hpp"
#include "core/states/state_machine.hpp"

#include <SDL.h>

namespace mswpr
{
  game_engine::game_engine(std::string_view title, size_t xpos, size_t ypos) :
    d_renderer(title, xpos, ypos),
    d_minefield(cfg::FIELD_WIDTH, cfg::FIELD_HEIGHT, cfg::MINES_CNT),
    d_face_type(face_type::SMILE_NOT_PRESSED),
    d_counter(cfg::MINES_CNT),
    d_state(d_minefield, d_face_type, d_counter, d_timer),
    d_frame_start_ticks(0),
    d_is_running(true)
  {
  }

  bool game_engine::running() const
  {
    return d_is_running;
  }

  void game_engine::handle_input()
  {
    d_frame_start_ticks = SDL_GetTicks();

    bool is_released = false;
    int key = -1;

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
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
          d_is_running = false;
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
          d_is_running = false;
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

    if (is_left_btn && d_renderer.is_inside_face(mouse_x, mouse_y))
    {
      d_state.on_left_face_click(is_released);
    }
    else if (d_renderer.is_inside_field(mouse_x, mouse_y))
    {
      const auto x = static_cast<int>((mouse_x - mswpr::layout::BOARD_OFFSET_X) / mswpr::layout::CELL_WIDTH);
      const auto y = static_cast<int>((mouse_y - mswpr::layout::BOARD_OFFSET_Y) / mswpr::layout::CELL_HEIGHT);
      if (is_left_btn)
      {
        d_state.on_left_field_click(is_released, x, y);
      }
      else if (is_right_btn)
      {
        d_state.on_right_field_click(is_released, x, y);
      }
    }
  }

  void game_engine::render()
  {
    d_renderer.render(d_minefield, d_face_type, d_counter, d_timer);
  }

  void game_engine::update()
  {
    d_timer.update(game_timer::now());
  }

  void game_engine::limit_fps() const
  {
    const auto frame_ticks = SDL_GetTicks() - d_frame_start_ticks;

    if (cfg::FRAME_DELAY > frame_ticks)
    {
      SDL_Delay(cfg::FRAME_DELAY - frame_ticks);
    }
  }

}  // namespace mswpr
