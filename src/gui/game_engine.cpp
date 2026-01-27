#include "gui/game_engine.hpp"
#include "core/game_config.hpp"
#include "core/states/state_machine.hpp"

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>

namespace mswpr
{
  game_engine::game_engine(std::string_view title, size_t xpos, size_t ypos) :
    d_renderer(title, xpos, ypos),
    d_minefield(cfg::FIELD_WIDTH, cfg::FIELD_HEIGHT, cfg::MINES_CNT),
    d_face_type(face_type::SMILE_NOT_PRESSED),
    d_counter(cfg::MINES_CNT),
    d_state(d_minefield, d_face_type, d_counter, d_timer, spdlog::get("game")),
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
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_EVENT_KEY_UP:
        is_released = true;
        [[fallthrough]];
      case SDL_EVENT_KEY_DOWN:
        switch (event.key.key)
        {
        case SDLK_ESCAPE:
          d_is_running = false;
          return;
        default:
          break;
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_UP:
        is_released = true;
        [[fallthrough]];
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        key = event.button.button;
        break;
      case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        d_is_running = false;
        return;
        break;
      default:
        break;
      }
    }

    process_click(is_released, key);
  }

  void game_engine::process_click(bool is_released, int key)
  {
    // TODO: #28 Refactor float to int comparision
    float mouse_x_float = 0;
    float mouse_y_float = 0;

    SDL_GetMouseState(&mouse_x_float, &mouse_y_float);
    int mouse_x = static_cast<int>(mouse_x_float);
    int mouse_y = static_cast<int>(mouse_y_float);
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
      const auto x = static_cast<int>((mouse_x - mswpr::layout::BORDER_WIDTH) / mswpr::layout::CELL_WIDTH);
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
    // TODO: #28 Remove Uint32 to Uint64 comparision
    const auto frame_ticks = static_cast<Uint32>(SDL_GetTicks() - d_frame_start_ticks);

    if (cfg::FRAME_DELAY > frame_ticks)
    {
      SDL_Delay(cfg::FRAME_DELAY - frame_ticks);
    }
  }

}  // namespace mswpr
