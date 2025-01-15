#include "core/game_timer.hpp"

#include <array>
#include <chrono>

#include <gtest/gtest.h>

using namespace std::chrono_literals;

namespace
{
  constexpr mswpr::game_timer::time_point_t INITIAL_TIME{};
}

TEST(GameTimer, Constructor)
{
  mswpr::game_timer timer;

  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(timer.get_elapsed_time(), 0s);
}

TEST(GameTimer, Start)
{
  mswpr::game_timer timer;

  timer.start(INITIAL_TIME);
  EXPECT_TRUE(timer.is_running());
  timer.update(INITIAL_TIME + 2s);

  EXPECT_TRUE(timer.is_running());
  EXPECT_EQ(timer.get_elapsed_time(), 2s);
}

TEST(GameTimer, Stop)
{
  mswpr::game_timer timer;

  timer.start(INITIAL_TIME);
  timer.update(INITIAL_TIME + 2s);
  timer.stop(INITIAL_TIME + 3s);

  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(timer.get_elapsed_time(), 3s);
}

TEST(GameTimer, Reset)
{
  mswpr::game_timer timer;

  timer.start(INITIAL_TIME);
  timer.stop(INITIAL_TIME + 3s);

  EXPECT_EQ(timer.get_elapsed_time(), 3s);
  timer.reset();
  EXPECT_FALSE(timer.is_running());
  EXPECT_EQ(timer.get_elapsed_time(), 0s);
}

TEST(GameTimer, UpdateOnNotStarted)
{
  mswpr::game_timer timer;

  timer.update(INITIAL_TIME + 2s);

  EXPECT_EQ(timer.get_elapsed_time(), 0s);
}

TEST(GameTimer, UpdateOnStopped)
{
  mswpr::game_timer timer;

  timer.start(INITIAL_TIME);
  timer.stop(INITIAL_TIME + 3s);

  EXPECT_EQ(timer.get_elapsed_time(), 3s);

  timer.update(INITIAL_TIME + 10s);

  EXPECT_EQ(timer.get_elapsed_time(), 3s);
}

TEST(GameTimer, StartAlreadyStarted)
{
  mswpr::game_timer timer;

  timer.start(INITIAL_TIME);
  EXPECT_EQ(timer.get_elapsed_time(), 0s);

  timer.start(INITIAL_TIME + 2s);
  EXPECT_EQ(timer.get_elapsed_time(), 0s);
}

TEST(GameTimer, StopAlreadyStopped)
{
  mswpr::game_timer timer;

  timer.start(INITIAL_TIME);
  timer.update(INITIAL_TIME + 3s);

  timer.stop(INITIAL_TIME + 5s);
  EXPECT_EQ(timer.get_elapsed_time(), 5s);

  timer.stop(INITIAL_TIME + 10s);
  EXPECT_EQ(timer.get_elapsed_time(), 5s);
}

TEST(GameTimer, SeveralUpdates)
{
  mswpr::game_timer timer;

  timer.start(INITIAL_TIME);
  for (size_t i = 1; i <= 5; ++i)
    timer.update(INITIAL_TIME + std::chrono::seconds(i));

  EXPECT_EQ(timer.get_elapsed_time(), 5s);
}

TEST(GameTimer, Now)
{
  const auto chrono_now = std::chrono::steady_clock::now();
  const auto timer_now = mswpr::game_timer::now();

  EXPECT_GE(timer_now, chrono_now);
}

namespace
{
  struct extract_digits_from_seconds_test_data
  {
    std::chrono::milliseconds elapsed_time;
    std::array<uint8_t, 3> expected_digits;
  };

  class as_digit_array_test : public testing::TestWithParam<extract_digits_from_seconds_test_data>
  {
  };

  constexpr std::array EXTRACT_DIGITS_FROM_SECONDS_TEST_SUITE = {
    extract_digits_from_seconds_test_data{ .elapsed_time = 0s, .expected_digits = { 0, 0, 0 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 2s, .expected_digits = { 0, 0, 2 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 17s, .expected_digits = { 0, 1, 7 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 478s, .expected_digits = { 4, 7, 8 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 5311s, .expected_digits = { 9, 9, 9 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 17101ms, .expected_digits = { 0, 1, 7 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 17701ms, .expected_digits = { 0, 1, 7 } },
  };
}

TEST_P(as_digit_array_test, CorrectConvertation)
{
  const auto& [elapsed_time, expected_digits] = GetParam();
  mswpr::game_timer timer;

  timer.start(INITIAL_TIME);
  timer.update(INITIAL_TIME + elapsed_time);

  const auto elapsed_time_in_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time);
  EXPECT_EQ(timer.get_elapsed_time(), elapsed_time_in_seconds);
  EXPECT_EQ(timer.extract_digits_from_seconds(), expected_digits);
}

INSTANTIATE_TEST_SUITE_P(GameTimer,
                         as_digit_array_test,
                         testing::ValuesIn(EXTRACT_DIGITS_FROM_SECONDS_TEST_SUITE),
                         [](const testing::TestParamInfo<as_digit_array_test::ParamType>& i_info) {
                           const auto& t = i_info.param.elapsed_time;
                           const std::string name = "Elapsed_" + std::to_string(t.count()) + "ms";
                           return name;
                         });
