#include "gui/game_renderer.hpp"
#include "core/game_config.hpp"
#include "gui/border_renderer.hpp"
#include "gui/game_timer_renderer.hpp"
#include "gui/game_version.hpp"
#include "gui/logger.hpp"
#include "gui/minefield_renderer.hpp"
#include "gui/mines_counter_renderer.hpp"
#include "gui/sdl_helper.hpp"

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>

#include <filesystem>

namespace
{
  bool is_inside(const SDL_Rect& rect, int x, int y)
  {
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
  }

  mswpr::sdl_window_t init_window(std::string_view title, size_t xpos, size_t ypos)
  {
    const size_t window_width =
      static_cast<size_t>(2) * mswpr::layout::BORDER_WIDTH + mswpr::layout::CELL_WIDTH * cfg::FIELD_WIDTH;
    const size_t window_height =
      mswpr::layout::BOARD_OFFSET_Y + mswpr::layout::BORDER_WIDTH + mswpr::layout::CELL_HEIGHT * cfg::FIELD_HEIGHT;
    spdlog::get("engine")->debug("Window size: {} x {}.", window_width, window_height);

    const SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title.data());
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, static_cast<int64_t>(xpos));
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, static_cast<int64_t>(ypos));
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, window_width);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, window_height);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true);

    auto* window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);

    return mswpr::sdl_window_t(window, mswpr::sdl_deleter{});
  }
}  // namespace

mswpr::game_renderer::game_renderer(std::string_view title, size_t xpos, size_t ypos) : d_face_rect(), d_field_rect()
{
  spdlog::get("engine")->info("mswpr version: {}.", mswpr::get_game_version());

  d_window = init_window(title, xpos, ypos);
  if (!d_window)
  {
    spdlog::get("engine")->error("Unable to create SDL_window: {}.", SDL_GetError());
    return;
  }

  spdlog::get("engine")->info("Window created.");

  d_renderer.reset(SDL_CreateRenderer(d_window.get(), nullptr), mswpr::sdl_deleter{});
  if (!d_renderer)
  {
    spdlog::get("engine")->error("Unable to create SDL_Renderer: {}.", SDL_GetError());
    return;
  }

  SDL_SetRenderDrawColor(d_renderer.get(), 190, 190, 190, 0);
  spdlog::get("engine")->info("Renderer created.");

  const std::filesystem::path path_to_binary = SDL_GetBasePath();
  const std::filesystem::path path_to_skin = path_to_binary / "assets/winxpskin.bmp";
  d_texture_manager.init(d_renderer, path_to_skin.string());

  const int face_x =
    cfg::FIELD_WIDTH * mswpr::layout::CELL_WIDTH / 2 - mswpr::layout::FACE_WIDTH / 2 + mswpr::layout::BORDER_WIDTH;
  d_face_rect = { face_x, mswpr::layout::HUD_OFFSET_Y, mswpr::layout::FACE_WIDTH, mswpr::layout::FACE_HEIGHT };

  d_field_rect = { mswpr::layout::BORDER_WIDTH,
                   mswpr::layout::BOARD_OFFSET_Y,
                   mswpr::layout::CELL_WIDTH * cfg::FIELD_WIDTH,
                   mswpr::layout::CELL_HEIGHT * cfg::FIELD_HEIGHT };

  if (!SDL_ShowWindow(d_window.get()))
  {
    spdlog::get("engine")->error("Unable to show SDL_Window: {}.", SDL_GetError());
    return;
  }
}

bool mswpr::game_renderer::is_inside_face(int mouse_x, int mouse_y)
{
  return is_inside(d_face_rect, mouse_x, mouse_y);
}

bool mswpr::game_renderer::is_inside_field(int mouse_x, int mouse_y)
{
  return is_inside(d_field_rect, mouse_x, mouse_y);
}

void mswpr::game_renderer::render(const mswpr::minefield& field,
                                  mswpr::face_type face,
                                  const mswpr::mines_counter& counter,
                                  const mswpr::game_timer& timer)
{
  SDL_RenderClear(d_renderer.get());

  mswpr::mines_counter_renderer counter_renderer(d_texture_manager);
  counter_renderer.draw(counter);

  mswpr::game_timer_renderer timer_renderer(d_texture_manager);
  timer_renderer.draw(timer);

  d_texture_manager.draw(face, d_face_rect);

  mswpr::minefield_renderer field_renderer(d_texture_manager);
  field_renderer.draw(field);

  mswpr::border_renderer border(d_texture_manager);
  border.draw();

  SDL_RenderPresent(d_renderer.get());
}
