#ifndef MSWPR_GAME_TIMER_RENDERER_HPP
#define MSWPR_GAME_TIMER_RENDERER_HPP

namespace mswpr
{
  class game_timer;
  class texture_manager;

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
