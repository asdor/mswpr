#include "core/game_timer.hpp"

#include <array>
#include <chrono>

#include <gtest/gtest.h>

using namespace std::chrono_literals;

TEST(GameTimer, Constructor)
{
  mswpr::game_timer timer;

  EXPECT_EQ(timer.get_elapsed_time(), 0s);
}
