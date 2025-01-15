#ifndef MSWPR_STATE_MACHINE_HPP
#define MSWPR_STATE_MACHINE_HPP

#include "core/states/ending_state.hpp"
#include "core/states/generating_state.hpp"
#include "core/states/playing_state.hpp"

#include "core/game_timer.hpp"
#include "core/minefield.hpp"
#include "core/mines_counter.hpp"
#include "core/types.hpp"

#include <variant>

namespace mswpr
{
  class state_machine
  {
  public:
    state_machine(mswpr::minefield& minefield,
                  mswpr::face_type& face_type,
                  mswpr::mines_counter& counter,
                  mswpr::game_timer& timer);

    template<class NewState, class... Args>
    void set_state(Args&&... args)
    {
      d_state.emplace<NewState>(*this, std::forward<Args>(args)...);
    }

    template<class St>
    constexpr bool is_in_state() const
    {
      return std::holds_alternative<St>(d_state);
    }

    void on_left_face_click(bool is_released);
    void on_left_field_click(bool is_released, size_t x, size_t y);
    void on_right_field_click(bool is_released, size_t x, size_t y);

    mswpr::minefield& get_field();
    void set_face(mswpr::face_type face);
    mswpr::mines_counter& get_counter();
    mswpr::game_timer& get_timer();

  private:
    mswpr::minefield& d_minefield;
    mswpr::face_type& d_face_type;
    mswpr::mines_counter& d_counter;
    mswpr::game_timer& d_timer;

    using state_t = std::variant<generating_state, playing_state, ending_state>;
    state_t d_state;
  };
}

#endif  // MSWPR_STATE_MACHINE_HPP
