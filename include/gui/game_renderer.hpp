#ifndef MSWPR_GAME_RENDERER_HPP
#define MSWPR_GAME_RENDERER_HPP

#include "core/game_timer.hpp"
#include "core/minefield.hpp"
#include "core/mines_counter.hpp"
#include "gui/sdl_helper.hpp"
#include "gui/texture_manager.hpp"

#include <string_view>

namespace mswpr
{
  class game_renderer
  {
  public:
    game_renderer(std::string_view title, size_t xpos, size_t ypos);

    void render(const mswpr::minefield& field,
                mswpr::face_type face,
                const mswpr::mines_counter& counter,
                const mswpr::game_timer& i_timer);

    bool is_inside_face(int mouse_x, int mouse_y);
    bool is_inside_field(int mouse_x, int mouse_y);

  private:
    void draw_field(const mswpr::minefield& field);
    void draw_mines_counter(const mswpr::mines_counter& counter);
    void draw_timer(const mswpr::game_timer& i_timer);

    mswpr::sdl_init_t d_sdl_init;
    mswpr::sdl_window_t d_window;
    mswpr::sdl_renderer_t d_renderer;

    mswpr::texture_manager d_texture_manager;

    SDL_Rect d_face_rect;
    SDL_Rect d_field_rect;
  };
}

#endif  // MSWPR_GAME_RENDERER_HPP
