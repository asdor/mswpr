#include "core/game_timer.hpp"
#include "core/minefield.hpp"
#include "core/mines_counter.hpp"
#include "core/states/generating_state.hpp"
#include "core/states/state_machine.hpp"
#include "core/types.hpp"
#include "test_utils/mocked_mines_generator.hpp"

#include <variant>
#include <vector>

#include <gtest/gtest.h>

namespace
{
  constexpr size_t FIELD_WIDTH = 5;
  constexpr size_t FIELD_HEIGHT = 5;
  constexpr bool RELEASED = true;
  constexpr bool PRESSED = false;

  const mswpr::unit_tests::MockedGenerator MOCKED_MINES_GENERATOR({ { 0, 0 } });
}

using namespace mswpr;

class StateMachineTransitionTest : public ::testing::Test
{
protected:
  StateMachineTransitionTest() :
    field_(FIELD_WIDTH, FIELD_HEIGHT, MOCKED_MINES_GENERATOR.get_mines_cnt()),
    face_(face_type::SMILE_NOT_PRESSED),
    counter_(field_.get_bomb_cnt()),
    st_machine_(field_, face_, counter_, timer_)
  {
    field_.generate(MOCKED_MINES_GENERATOR);
  }

  template<class NewState, class... Args>
  void change_state(Args&&... args)
  {
    st_machine_.set_state<NewState>(std::forward<Args>(args)...);
    ASSERT_TRUE(st_machine_.is_in_state<NewState>());
  }

  minefield field_;
  face_type face_;
  mines_counter counter_;
  game_timer timer_;
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
  EXPECT_FALSE(timer_.is_running());
  st_machine_.on_left_field_click(PRESSED, 0, 0);
  ASSERT_EQ(face_, face_type::WAITING);

  st_machine_.on_left_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine_.is_in_state<playing_state>());
  EXPECT_TRUE(timer_.is_running());
}

TEST_F(StateMachineTransitionTest, FromGenerating_OnRightFieldClick)
{
  st_machine_.on_right_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine_.is_in_state<generating_state>());
}

TEST_F(StateMachineTransitionTest, FromPlaying_OnLeftFaceClick)
{
  change_state<playing_state>();
  EXPECT_TRUE(timer_.is_running());

  st_machine_.on_left_face_click(PRESSED);
  ASSERT_EQ(face_, face_type::SMILE_PRESSED);

  st_machine_.on_left_face_click(RELEASED);
  ASSERT_TRUE(st_machine_.is_in_state<generating_state>());
  EXPECT_FALSE(timer_.is_running());
}

TEST_F(StateMachineTransitionTest, FromPlaying_OnLeftFaceClick_ResetCounter)
{
  change_state<playing_state>();

  counter_.reset(0);
  EXPECT_EQ(counter_.get_value(), 0);
  st_machine_.on_left_face_click(PRESSED);

  st_machine_.on_left_face_click(RELEASED);
  ASSERT_TRUE(st_machine_.is_in_state<generating_state>());
  EXPECT_EQ(counter_.get_value(), 1);
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
  EXPECT_TRUE(timer_.is_running());
  ASSERT_EQ(field_.get_value(x, y), 0);
  EXPECT_EQ(counter_.get_value(), 1);

  st_machine_.on_left_field_click(RELEASED, x, y);
  ASSERT_TRUE(st_machine_.is_in_state<ending_state>());
  EXPECT_FALSE(timer_.is_running());
  EXPECT_EQ(face_, face_type::BOSS);
  EXPECT_EQ(counter_.get_value(), 0);
}

TEST_F(StateMachineTransitionTest, FromPlaying_OnLeftFieldClick_OnBomb)
{
  const size_t x = 0;
  const size_t y = 0;
  change_state<playing_state>();
  EXPECT_TRUE(timer_.is_running());
  ASSERT_TRUE(field_(x, y).is_bomb());

  st_machine_.on_left_field_click(RELEASED, x, y);
  ASSERT_TRUE(st_machine_.is_in_state<ending_state>());
  EXPECT_FALSE(timer_.is_running());
  EXPECT_EQ(face_, face_type::DEAD);
}

TEST_F(StateMachineTransitionTest, FromPlaying_OnRightFieldClick)
{
  change_state<playing_state>();

  st_machine_.on_right_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine_.is_in_state<playing_state>());
}

TEST_F(StateMachineTransitionTest, FromEnding_OnLeftFaceClick)
{
  change_state<ending_state>(ending_state::params{ .is_victory = true });
  EXPECT_FALSE(timer_.is_running());

  st_machine_.on_left_face_click(RELEASED);
  ASSERT_TRUE(st_machine_.is_in_state<generating_state>());
  EXPECT_FALSE(timer_.is_running());
}

TEST_F(StateMachineTransitionTest, FromEnding_OnLeftFieldClick)
{
  change_state<ending_state>(ending_state::params{ .is_victory = true });

  st_machine_.on_left_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine_.is_in_state<ending_state>());
}

TEST_F(StateMachineTransitionTest, FromEnding_OnRightFieldClick)
{
  change_state<ending_state>(ending_state::params{ .is_victory = true });

  st_machine_.on_right_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine_.is_in_state<ending_state>());
}

TEST_F(StateMachineTransitionTest, FromEnding_Victory)
{
  change_state<ending_state>(ending_state::params{ .is_victory = true });
  EXPECT_EQ(face_, face_type::BOSS);
}

TEST_F(StateMachineTransitionTest, FromEnding_Defeat)
{
  change_state<ending_state>(ending_state::params{ .is_victory = false });
  EXPECT_EQ(face_, face_type::DEAD);
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
