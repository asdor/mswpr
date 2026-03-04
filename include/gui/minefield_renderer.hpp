#ifndef MSWPR_MINEFIELD_RENDERER_HPP
#define MSWPR_MINEFIELD_RENDERER_HPP

#include "core/fwd.hpp"
#include "gui/fwd.hpp"

namespace mswpr
{
  class minefield_renderer
  {
  public:
    minefield_renderer(mswpr::texture_manager& texture_manager);
    void draw(const minefield& field);

  private:
    mswpr::texture_manager& d_texture_manager;
  };
}

#endif  // MSWPR_MINEFIELD_RENDERER_HPP
