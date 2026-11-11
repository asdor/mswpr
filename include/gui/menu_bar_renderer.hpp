#ifndef MSWPR_MENU_BAR_RENDERER_HPP
#define MSWPR_MENU_BAR_RENDERER_HPP

#include "gui/sdl_helper.hpp"

namespace mswpr
{

  enum class menu_clicked_item
  {
    NONE,
    NEW_GAME,
    EXIT,
  };

  class menu_bar_renderer
  {
  public:
    menu_bar_renderer();

    void draw(mswpr::sdl_renderer_t renderer);

    int get_menu_bar_height() const;

    menu_clicked_item get_clicked_item() const;
    void set_menu_bar_style() const;

  private:
    void draw_menu_bar();

    menu_clicked_item d_clicked_item;
  };

}  // namespace mswpr

#endif  // MSWPR_MENU_BAR_RENDERER_HPP
