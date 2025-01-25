#ifndef MSWPR_STATE_INTERFACE_HPP
#define MSWPR_STATE_INTERFACE_HPP

#include <spdlog/fwd.h>

#include <cstddef>
#include <memory>

namespace mswpr
{
  class state_machine;

  enum class face_type : size_t;

  class state_interface
  {
  public:
    state_interface(state_machine& st_machine);

    void on_left_face_click(bool is_released);
    void on_left_field_click(bool is_released, size_t x, size_t y);
    void on_right_field_click(bool is_released, size_t x, size_t y);

  protected:
    bool change_face_on_click(bool is_released, face_type pressed, face_type released);

    state_machine& get_state_machine();

    std::shared_ptr<spdlog::logger> get_logger();

  private:
    state_machine& d_st_machine;
  };
}  // namespace mswpr

#endif  // MSWPR_STATE_INTERFACE_HPP
