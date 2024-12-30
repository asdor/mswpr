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

TEST(GameTimer, SeveralUpdates)
{
  mswpr::game_timer timer;

  timer.start(initial_time);
  for (size_t i = 1; i <= 5; ++i)
    timer.update(initial_time + std::chrono::seconds(i));

  EXPECT_EQ(timer.get_elapsed_time(), 5s);
}
