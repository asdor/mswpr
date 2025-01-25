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
#include <spdlog/spdlog.h>

namespace
{
  constexpr size_t FIELD_WIDTH = 5;
  constexpr size_t FIELD_HEIGHT = 5;
  constexpr bool RELEASED = true;
  constexpr bool PRESSED = false;

  auto init_test_logger()
  {
    spdlog::set_level(spdlog::level::off);
    return spdlog::default_logger();
  }
}

class state_machine_transition_test : public ::testing::Test
{
protected:
  state_machine_transition_test() :
    mocked_mines_generator({ { 0, 0 } }),
    field(FIELD_WIDTH, FIELD_HEIGHT, mocked_mines_generator.get_mines_cnt()),
    face(mswpr::face_type::SMILE_NOT_PRESSED),
    counter(field.get_bomb_cnt()),
    st_machine(field, face, counter, timer, init_test_logger())
  {
    field.generate(mocked_mines_generator);
  }

  template<class NewState, class... Args>
  void change_state(Args&&... args)
  {
    st_machine.set_state<NewState>(std::forward<Args>(args)...);
    ASSERT_TRUE(st_machine.is_in_state<NewState>());
  }

public:
  mswpr::unit_tests::mocked_generator mocked_mines_generator;
  mswpr::minefield field;
  mswpr::face_type face;
  mswpr::mines_counter counter;
  mswpr::game_timer timer;
  mswpr::state_machine st_machine;
};

TEST_F(state_machine_transition_test, DefaultState)
{
  ASSERT_TRUE(st_machine.is_in_state<mswpr::generating_state>());
}

TEST_F(state_machine_transition_test, FromGenerating_OnLeftFaceClick)
{
  st_machine.on_left_face_click(PRESSED);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::generating_state>());
  ASSERT_EQ(face, mswpr::face_type::SMILE_PRESSED);

  st_machine.on_left_face_click(RELEASED);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::generating_state>());
  ASSERT_EQ(face, mswpr::face_type::SMILE_NOT_PRESSED);
}

TEST_F(state_machine_transition_test, FromGenerating_OnLeftFieldClick)
{
  EXPECT_FALSE(timer.is_running());
  st_machine.on_left_field_click(PRESSED, 0, 0);
  ASSERT_EQ(face, mswpr::face_type::WAITING);

  st_machine.on_left_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::playing_state>());
  EXPECT_TRUE(timer.is_running());
}

TEST_F(state_machine_transition_test, FromGenerating_OnRightFieldClick)
{
  st_machine.on_right_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::generating_state>());
}

TEST_F(state_machine_transition_test, FromPlaying_OnLeftFaceClick)
{
  change_state<mswpr::playing_state>();
  EXPECT_TRUE(timer.is_running());

  st_machine.on_left_face_click(PRESSED);
  ASSERT_EQ(face, mswpr::face_type::SMILE_PRESSED);

  st_machine.on_left_face_click(RELEASED);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::generating_state>());
  EXPECT_FALSE(timer.is_running());
}

TEST_F(state_machine_transition_test, FromPlaying_OnLeftFaceClick_ResetCounter)
{
  change_state<mswpr::playing_state>();

  counter.reset(0);
  EXPECT_EQ(counter.get_value(), 0);
  st_machine.on_left_face_click(PRESSED);

  st_machine.on_left_face_click(RELEASED);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::generating_state>());
  EXPECT_EQ(counter.get_value(), 1);
}

TEST_F(state_machine_transition_test, FromPlaying_OnLeftFieldClick_OnEmpty)
{
  const size_t x = 1;
  const size_t y = 1;
  change_state<mswpr::playing_state>();
  ASSERT_EQ(field.get_value(x, y), 1);

  st_machine.on_left_field_click(RELEASED, x, y);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::playing_state>());
}

TEST_F(state_machine_transition_test, FromPlaying_OnLeftFieldClick_Demined)
{
  const size_t x = 4;
  const size_t y = 4;
  change_state<mswpr::playing_state>();
  EXPECT_TRUE(timer.is_running());
  ASSERT_EQ(field.get_value(x, y), 0);
  EXPECT_EQ(counter.get_value(), 1);

  st_machine.on_left_field_click(RELEASED, x, y);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::ending_state>());
  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(face, mswpr::face_type::BOSS);
  EXPECT_EQ(counter.get_value(), 0);
}

TEST_F(state_machine_transition_test, FromPlaying_OnLeftFieldClick_OnBomb)
{
  const size_t x = 0;
  const size_t y = 0;
  change_state<mswpr::playing_state>();
  EXPECT_TRUE(timer.is_running());
  ASSERT_TRUE(field(x, y).is_bomb());

  st_machine.on_left_field_click(RELEASED, x, y);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::ending_state>());
  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(face, mswpr::face_type::DEAD);
}

TEST_F(state_machine_transition_test, FromPlaying_OnRightFieldClick)
{
  change_state<mswpr::playing_state>();

  st_machine.on_right_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::playing_state>());
}

TEST_F(state_machine_transition_test, FromEnding_OnLeftFaceClick)
{
  change_state<mswpr::ending_state>(mswpr::ending_state::params{ .is_victory = true });
  EXPECT_FALSE(timer.is_running());

  st_machine.on_left_face_click(RELEASED);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::generating_state>());
  EXPECT_FALSE(timer.is_running());
}

TEST_F(state_machine_transition_test, FromEnding_OnLeftFieldClick)
{
  change_state<mswpr::ending_state>(mswpr::ending_state::params{ .is_victory = true });

  st_machine.on_left_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::ending_state>());
}

TEST_F(state_machine_transition_test, FromEnding_OnRightFieldClick)
{
  change_state<mswpr::ending_state>(mswpr::ending_state::params{ .is_victory = true });

  st_machine.on_right_field_click(RELEASED, 0, 0);
  ASSERT_TRUE(st_machine.is_in_state<mswpr::ending_state>());
}

TEST_F(state_machine_transition_test, FromEnding_Victory)
{
  change_state<mswpr::ending_state>(mswpr::ending_state::params{ .is_victory = true });
  EXPECT_EQ(face, mswpr::face_type::BOSS);
}

TEST_F(state_machine_transition_test, FromEnding_Defeat)
{
  change_state<mswpr::ending_state>(mswpr::ending_state::params{ .is_victory = false });
  EXPECT_EQ(face, mswpr::face_type::DEAD);
}

TEST_F(state_machine_transition_test, FromGenerating_OnRightFieldClick_PlayCounter)
{
  ASSERT_EQ(counter.get_value(), 1);

  st_machine.on_right_field_click(RELEASED, 0, 0);
  ASSERT_EQ(counter.get_value(), 0);

  st_machine.on_right_field_click(RELEASED, 0, 0);
  ASSERT_EQ(counter.get_value(), 1);
}

TEST_F(state_machine_transition_test, FromPlaying_OnRightFieldClick_PlayCounter)
{
  change_state<mswpr::playing_state>();

  ASSERT_EQ(counter.get_value(), 1);

  st_machine.on_right_field_click(RELEASED, 0, 0);
  ASSERT_EQ(counter.get_value(), 0);

  st_machine.on_right_field_click(RELEASED, 0, 0);
  ASSERT_EQ(counter.get_value(), 1);
}
