#ifndef MSWPR_GAME_ENGINE_HPP
#define MSWPR_GAME_ENGINE_HPP

#include "gui/game_renderer.hpp"
#include "core/game_timer.hpp"
#include "core/minefield.hpp"
#include "core/mines_counter.hpp"
#include "core/states/state_machine.hpp"
#include "core/states/ending_state.hpp"
#include "core/states/generating_state.hpp"
#include "core/states/playing_state.hpp"

#include <string_view>

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

    bool is_running_;

    mswpr::game_renderer renderer_;

    mswpr::minefield minefield_;
    mswpr::face_type face_type_;
    mswpr::mines_counter counter_;
    mswpr::game_timer timer_;

    mswpr::state_machine state_;

    Uint32 frame_start_ticks_;
  };

}  // mswpr

#endif  // MSWPR_GAME_ENGINE_HPP
