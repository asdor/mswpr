#include "core/minefield.hpp"
#include "core/types.hpp"
#include "core/mines_counter.hpp"
#include "core/states/state_machine.hpp"
#include "core/states/generating_state.hpp"

#include <variant>
#include <vector>

#include <gtest/gtest.h>

namespace
{
  constexpr size_t FIELD_WIDTH = 5;
  constexpr size_t FIELD_HEIGHT = 5;
  const std::vector<size_t> MINES = { 0 };
  constexpr bool RELEASED = true;
  constexpr bool PRESSED = false;
}

using namespace mswpr;

class StateMachineTransitionTest : public ::testing::Test
{
protected:
  StateMachineTransitionTest() :
    field_(MINES, FIELD_WIDTH, FIELD_HEIGHT, MINES.size()),
    face_(face_type::SMILE_NOT_PRESSED),
    counter_(MINES.size()),
    st_machine_(field_, face_, counter_)
  {
  }

  template<class NewState, class... Args>
  void change_state(Args&&... args)
  {
    st_machine_.set_state<NewState>(std::forward<Args>(args)...);
    field_ = minefield(MINES, FIELD_WIDTH, FIELD_HEIGHT, MINES.size());
    ASSERT_TRUE(st_machine_.is_in_state<NewState>());
  }

  minefield field_;
  face_type face_;
  mines_counter counter_;
  state_machine st_machine_;
};

TEST_F(StateMachineTransitionTest, DefaultState)
{
  ASSERT_TRUE(st_machine_.is_in_state<generating_state>());
}

TEST_F(StateMachineTransitionTest, FromGenerating_OnLeftFaceClick)
{
  st_machine_.on_left_face_click(PRESSED);
  ASSERT_TRUE(st_machine_.is_in_state<generating_state>());
  ASSERT_EQ(face_, face_type::SMILE_PRESSED);

  st_machine_.on_left_face_click(RELEASED);
  ASSERT_TRUE(st_machine_.is_in_state<generating_state>());
  ASSERT_EQ(face_, face_type::SMILE_NOT_PRESSED);
}

TEST_F(StateMachineTransitionTest, FromGenerating_OnLeftFieldClick)
{
  st_machine_.on_left_field_click(PRESSED, 0, 0);
  ASSERT_EQ(face_, face_type::WAITING);

  st_machine_.on_left_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine_.is_in_state<playing_state>());
}

TEST_F(StateMachineTransitionTest, FromGenerating_OnRightFieldClick)
{
  st_machine_.on_right_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine_.is_in_state<generating_state>());
}

TEST_F(StateMachineTransitionTest, FromPlaying_OnLeftFaceClick)
{
  change_state<playing_state>();

  st_machine_.on_left_face_click(PRESSED);
  ASSERT_EQ(face_, face_type::SMILE_PRESSED);

  st_machine_.on_left_face_click(RELEASED);
  ASSERT_TRUE(st_machine_.is_in_state<generating_state>());
}

TEST_F(StateMachineTransitionTest, FromPlaying_OnLeftFieldClick_OnEmpty)
{
  const size_t x = 1;
  const size_t y = 1;
  change_state<playing_state>();
  ASSERT_EQ(field_.get_value(x, y), 1);

  st_machine_.on_left_field_click(RELEASED, x, y);
  ASSERT_TRUE(st_machine_.is_in_state<playing_state>());
}

TEST_F(StateMachineTransitionTest, FromPlaying_OnLeftFieldClick_Demined)
{
  const size_t x = 4;
  const size_t y = 4;
  change_state<playing_state>();
  ASSERT_EQ(field_.get_value(x, y), 0);

  st_machine_.on_left_field_click(RELEASED, x, y);
  ASSERT_TRUE(st_machine_.is_in_state<ending_state>());
}

TEST_F(StateMachineTransitionTest, FromPlaying_OnLeftFieldClick_OnBomb)
{
  const size_t x = 0;
  const size_t y = 0;
  change_state<playing_state>();
  ASSERT_TRUE(field_.is_bomb(x, y));

  st_machine_.on_left_field_click(RELEASED, x, y);
  ASSERT_TRUE(st_machine_.is_in_state<ending_state>());
}

TEST_F(StateMachineTransitionTest, FromPlaying_OnRightFieldClick)
{
  change_state<playing_state>();

  st_machine_.on_right_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine_.is_in_state<playing_state>());
}

TEST_F(StateMachineTransitionTest, FromEnding_OnLeftFaceClick)
{
  change_state<ending_state>();

  st_machine_.on_left_face_click(RELEASED);
  ASSERT_TRUE(st_machine_.is_in_state<generating_state>());
}

TEST_F(StateMachineTransitionTest, FromEnding_OnLeftFieldClick)
{
  change_state<ending_state>();

  st_machine_.on_left_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine_.is_in_state<ending_state>());
}

TEST_F(StateMachineTransitionTest, FromEnding_OnRightFieldClick)
{
  change_state<ending_state>();

  st_machine_.on_right_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine_.is_in_state<ending_state>());
}

TEST_F(StateMachineTransitionTest, FromGenerating_OnRightFieldClick_PlayCounter)
{
  ASSERT_EQ(counter_.get_value(), 1);

  st_machine_.on_right_field_click(RELEASED, 0, 0);
  ASSERT_EQ(counter_.get_value(), 0);

  st_machine_.on_right_field_click(RELEASED, 0, 0);
  ASSERT_EQ(counter_.get_value(), 1);
}

TEST_F(StateMachineTransitionTest, FromPlaying_OnRightFieldClick_PlayCounter)
{
  change_state<playing_state>();

  ASSERT_EQ(counter_.get_value(), 1);

  st_machine_.on_right_field_click(RELEASED, 0, 0);
  ASSERT_EQ(counter_.get_value(), 0);

  st_machine_.on_right_field_click(RELEASED, 0, 0);
  ASSERT_EQ(counter_.get_value(), 1);
}
