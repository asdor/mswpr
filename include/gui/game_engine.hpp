#ifndef MSWPR_GAME_ENGINE_HPP
#define MSWPR_GAME_ENGINE_HPP

#include "core/game_timer.hpp"
#include "core/minefield.hpp"
#include "core/mines_counter.hpp"
#include "core/states/ending_state.hpp"
#include "core/states/generating_state.hpp"
#include "core/states/playing_state.hpp"
#include "core/states/state_machine.hpp"
#include "gui/game_renderer.hpp"

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
    void limit_fps() const;

  private:
    void process_click(bool is_released, int key);

    mswpr::game_renderer d_renderer;

    mswpr::minefield d_minefield;
    mswpr::face_type d_face_type;
    mswpr::mines_counter d_counter;
    mswpr::game_timer d_timer;

    mswpr::state_machine d_state;

    Uint32 d_frame_start_ticks;

    bool d_is_running;
  };

}  // namespace mswpr

#endif  // MSWPR_GAME_ENGINE_HPP
