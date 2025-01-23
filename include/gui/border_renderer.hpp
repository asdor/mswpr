#ifndef MSWPR_BORDER_RENDERER_HPP
#define MSWPR_BORDER_RENDERER_HPP

#include "core/types.hpp"
#include "gui/texture_manager.hpp"

#include <optional>

namespace mswpr
{
  class border_renderer
  {
  public:
    border_renderer(mswpr::texture_manager& i_texture_manager);

    void draw();

  private:
    void draw_row(std::size_t i_row_y_pos,
                  std::size_t i_row_height,
                  mswpr::border_type i_left_segment,
                  std::optional<mswpr::border_type> i_middle_segment,
                  mswpr::border_type i_right_segment);

    mswpr::texture_manager& d_texture_manager;
  };

}  // namespace mswpr

#endif  // MSWPR_BORDER_RENDERER_HPP
