#include "gui/menu_bar_renderer.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

namespace
{
  constexpr ImColor WHITE_COLOR{ 255, 255, 255 };
  constexpr ImColor BLACK_COLOR{ 25, 25, 25 };
  constexpr ImColor GRAY_COLOR{ 245, 245, 245 };
}  // namespace

mswpr::menu_bar_renderer::menu_bar_renderer()
{
  set_menu_bar_style();
}

void mswpr::menu_bar_renderer::set_menu_bar_style() const
{
  ImGuiStyle* style = &ImGui::GetStyle();
  style->Colors[ImGuiCol_MenuBarBg] = WHITE_COLOR;
  style->Colors[ImGuiCol_Text] = BLACK_COLOR;
  style->Colors[ImGuiCol_HeaderHovered] = GRAY_COLOR;
  style->Colors[ImGuiCol_PopupBg] = WHITE_COLOR;
  style->WindowBorderSize = 0.0f;
}

int mswpr::menu_bar_renderer::get_menu_bar_height() const
{
  const float menu_bar_frame_height = ImGui::GetFrameHeight();
  return static_cast<int>(menu_bar_frame_height);
}

void mswpr::menu_bar_renderer::draw_menu_bar()
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("Game"))
    {
      if (ImGui::MenuItem("New"))
      {
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Beginner"))
      {
      }

      if (ImGui::MenuItem("Intermediate"))
      {
      }

      if (ImGui::MenuItem("Expert"))
      {
      }

      ImGui::Separator();

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

    if (ImGui::BeginMenu("Extras"))
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
