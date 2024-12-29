#ifndef MSWPR_GAME_RENDERER_HPP
#define MSWPR_GAME_RENDERER_HPP

#include "gui/sdl_helper.hpp"
#include "gui/texture_manager.hpp"
#include "core/minefield.hpp"
#include "core/mines_counter.hpp"

#include <string_view>

namespace mswpr
{
  class game_renderer
  {
  public:
    game_renderer(std::string_view title, size_t xpos, size_t ypos);

    void render(const mswpr::minefield& field, mswpr::face_type face, const mswpr::mines_counter& counter);

    bool is_inside_face(int mouse_x, int mouse_y);
    bool is_inside_field(int mouse_x, int mouse_y);

  private:
    void draw_field(const mswpr::minefield& field);
    void draw_mines_counter(const mswpr::mines_counter& counter);
    void draw_timer();

    mswpr::sdl_init_t sdl_init_;
    mswpr::sdl_window_t window_;
    mswpr::sdl_renderer_t renderer_;

    mswpr::texture_manager texture_manager_;

    SDL_Rect face_rect_;
    SDL_Rect field_rect_;
  };
}

#endif  // MSWPR_GAME_RENDERER_HPP
