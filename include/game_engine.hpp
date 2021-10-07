#ifndef MSWPR_GAME_ENGINE_HPP
#define MSWPR_GAME_ENGINE_HPP

#include <string_view>
#include <variant>

#include "sdl_helper.hpp"
#include "minefield.hpp"
#include "texture_manager.hpp"
#include "states/state_machine.hpp"
#include "states/ending_state.hpp"
#include "states/generating_state.hpp"
#include "states/playing_state.hpp"

namespace mswpr
{
  class game_engine
  {
  public:
    game_engine(std::string_view title, size_t xpos, size_t ypos);

    bool running() const;
    void handle_input();
    void update();
    void render();
    void limit_fps();

  private:
    void process_click(bool is_released, int key);

    mswpr::sdl_init_t sdl_init_;
    mswpr::sdl_window_t window_;
    mswpr::sdl_renderer_t renderer_;

    SDL_Rect face_rect_;
    SDL_Rect field_rect_;

    bool is_running_;
    mswpr::texture_manager texture_manager_;
    mswpr::minefield minefield_;
    mswpr::face_type face_type_;

    state_machine state_;

    Uint32 frame_start_ticks_;
  };

}  // mswpr

#endif  // MSWPR_GAME_ENGINE_HPP
