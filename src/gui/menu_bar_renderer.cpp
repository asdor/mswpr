#include "gui/menu_bar_renderer.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

void mswpr::menu_bar_renderer::draw_menu_bar()
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("Game"))
    {
      if (ImGui::MenuItem("New"))
      {
      }

      if (ImGui::MenuItem("Exit"))
      {
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("Dummy item"))
      {
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Extra"))
    {
      if (ImGui::MenuItem("Dummy item"))
      {
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

void mswpr::menu_bar_renderer::draw(mswpr::sdl_renderer_t renderer)
{
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  draw_menu_bar();

  ImGui::Render();
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer.get());
}
