#ifndef MSWPR_GAME_TIMER_RENDERER_HPP
#define MSWPR_GAME_TIMER_RENDERER_HPP

#include "core/fwd.hpp"
#include "gui/fwd.hpp"

namespace mswpr
{
  class game_timer_renderer
  {
  public:
    game_timer_renderer(mswpr::texture_manager& texture_manager);
    void draw(const game_timer& timer);

  private:
    mswpr::texture_manager& d_texture_manager;
  };
}

#endif  // MSWPR_GAME_TIMER_RENDERER_HPP
