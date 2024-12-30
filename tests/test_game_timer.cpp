#include "core/game_timer.hpp"

#include <array>
#include <chrono>

#include <gtest/gtest.h>

using namespace std::chrono_literals;

namespace
{
  constexpr mswpr::game_timer::TimePoint initial_time{};
}

TEST(GameTimer, Constructor)
{
  mswpr::game_timer timer;

  EXPECT_EQ(timer.get_elapsed_time(), 0s);
}

TEST(GameTimer, Start)
{
  mswpr::game_timer timer;

  timer.start(initial_time);
  timer.update(initial_time + 2s);

  EXPECT_EQ(timer.get_elapsed_time(), 2s);
}

TEST(GameTimer, Stop)
{
  mswpr::game_timer timer;

  timer.start(initial_time);
  timer.update(initial_time + 2s);
  timer.stop(initial_time + 3s);

  EXPECT_EQ(timer.get_elapsed_time(), 3s);
}

TEST(GameTimer, UpdateOnNotStarted)
{
  mswpr::game_timer timer;

  timer.update(initial_time + 2s);

  EXPECT_EQ(timer.get_elapsed_time(), 0s);
}

TEST(GameTimer, UpdateOnStopped)
{
  mswpr::game_timer timer;

  timer.start(initial_time);
  timer.stop(initial_time + 3s);

  EXPECT_EQ(timer.get_elapsed_time(), 3s);

  timer.update(initial_time + 10s);

  EXPECT_EQ(timer.get_elapsed_time(), 3s);
}

TEST(GameTimer, StartAlreadyStarted)
{
  mswpr::game_timer timer;

  timer.start(initial_time);
  EXPECT_EQ(timer.get_elapsed_time(), 0s);

  timer.start(initial_time + 2s);
  EXPECT_EQ(timer.get_elapsed_time(), 0s);
}

TEST(GameTimer, StopAlreadyStopped)
{
  mswpr::game_timer timer;

  timer.start(initial_time);
  timer.update(initial_time + 3s);

  timer.stop(initial_time + 5s);
  EXPECT_EQ(timer.get_elapsed_time(), 5s);

  timer.stop(initial_time + 10s);
  EXPECT_EQ(timer.get_elapsed_time(), 5s);
}

TEST(GameTimer, SeveralUpdates)
{
  mswpr::game_timer timer;

  timer.start(initial_time);
  for (size_t i = 1; i <= 5; ++i)
    timer.update(initial_time + std::chrono::seconds(i));

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

  class AsDigitArrayTest : public testing::TestWithParam<extract_digits_from_seconds_test_data>
  {
  };

  constexpr std::array extract_digits_from_seconds_test_suite = {
    extract_digits_from_seconds_test_data{ .elapsed_time = 0s, .expected_digits = { 0, 0, 0 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 2s, .expected_digits = { 0, 0, 2 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 17s, .expected_digits = { 0, 1, 7 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 478s, .expected_digits = { 4, 7, 8 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 5311s, .expected_digits = { 9, 9, 9 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 17101ms, .expected_digits = { 0, 1, 7 } },
    extract_digits_from_seconds_test_data{ .elapsed_time = 17701ms, .expected_digits = { 0, 1, 7 } },
  };
}

TEST_P(AsDigitArrayTest, CorrectConvertation)
{
  const auto& [elapsed_time, expected_digits] = GetParam();
  mswpr::game_timer timer;

  timer.start(initial_time);
  timer.update(initial_time + elapsed_time);

  const auto elapsed_time_in_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time);
  EXPECT_EQ(timer.get_elapsed_time(), elapsed_time_in_seconds);
  EXPECT_EQ(timer.extract_digits_from_seconds(), expected_digits);
}

INSTANTIATE_TEST_SUITE_P(GameTimer,
                         AsDigitArrayTest,
                         testing::ValuesIn(extract_digits_from_seconds_test_suite),
                         [](const testing::TestParamInfo<AsDigitArrayTest::ParamType>& i_info) {
                           const auto& t = i_info.param.elapsed_time;
                           const std::string name = "Elapsed_" + std::to_string(t.count()) + "ms";
                           return name;
                         });
