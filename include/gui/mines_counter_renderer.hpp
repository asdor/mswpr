#ifndef MSWPR_MINES_COUNTER_RENDERER_HPP
#define MSWPR_MINES_COUNTER_RENDERER_HPP

namespace mswpr
{
  class mines_counter;
  class texture_manager;

  class mines_counter_renderer
  {
  public:
    mines_counter_renderer(mswpr::texture_manager& texture_manager);
    void draw(const mines_counter& counter);

  private:
    mswpr::texture_manager& d_texture_manager;
  };
}

#endif  // MSWPR_MINES_COUNTER_RENDERER_HPP
