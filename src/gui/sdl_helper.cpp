#include "gui/sdl_helper.hpp"
#include "gui/logger.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <spdlog/spdlog.h>

namespace
{
  constexpr Uint32 INIT_FLAG = SDL_INIT_VIDEO;
}

namespace mswpr
{
  sdl_init_t::sdl_init_t()
  {
    if (!SDL_Init(INIT_FLAG))
      spdlog::get("engine")->error("Unable to initialize SDL: {}.", SDL_GetError());
  }

  mswpr::sdl_init_t::~sdl_init_t()
  {
    spdlog::get("engine")->debug("Shuting down SDL.");
    SDL_Quit();
  }

  void sdl_deleter::operator()(SDL_Texture* tex) const
  {
    spdlog::get("engine")->debug("Destroying SDL_Texture.");
    SDL_DestroyTexture(tex);
  }

  void sdl_deleter::operator()(SDL_Window* win) const
  {
    spdlog::get("engine")->debug("Destroying SDL_Window.");
    SDL_DestroyWindow(win);
  }

  void sdl_deleter::operator()(SDL_Renderer* ren) const
  {
    spdlog::get("engine")->debug("Destroying SDL_Renderer.");
    SDL_DestroyRenderer(ren);
  }

  imgui_context_raii::imgui_context_raii(sdl_window_t window, sdl_renderer_t renderer)
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    ImGui_ImplSDL3_InitForSDLRenderer(window.get(), renderer.get());
    ImGui_ImplSDLRenderer3_Init(renderer.get());
  }

  imgui_context_raii::~imgui_context_raii()
  {
    spdlog::get("engine")->debug("Destroying ImGui context.");
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
  }
}  // namespace mswpr
