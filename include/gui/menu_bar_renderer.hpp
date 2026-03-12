#ifndef MSWPR_MENU_BAR_RENDERER_HPP
#define MSWPR_MENU_BAR_RENDERER_HPP

#include "gui/sdl_helper.hpp"

namespace mswpr
{
  class menu_bar_renderer
  {
  public:
    menu_bar_renderer();

    void draw(mswpr::sdl_renderer_t renderer);

    int get_menu_bar_height() const;

  private:
    static void draw_menu_bar();
    void set_menu_bar_style() const;
  };

}  // namespace mswpr

#endif  // MSWPR_MENU_BAR_RENDERER_HPP
