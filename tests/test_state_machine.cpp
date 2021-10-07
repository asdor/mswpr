#include "core/minefield.hpp"
#include "core/types.hpp"
#include "core/states/state_machine.hpp"
#include "core/states/generating_state.hpp"

#include <gtest/gtest.h>

using namespace mswpr;

TEST(StateMachineTransition, FromGeneration)
{
  minefield field(5, 5, 1);
  face_type face = face_type::SMILE_CLOSED;
  state_machine st_machine(field, face);

  st_machine.on_left_face_click(true);

  ASSERT_TRUE(st_machine.is_in_state<generating_state>());
}
