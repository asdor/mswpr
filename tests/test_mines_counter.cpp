#include "core/mines_counter.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace mswpr;

namespace
{
  constexpr int MINES = 10;
}

TEST(MinesCounter, Constructor)
{
  mines_counter counter(MINES);

  EXPECT_EQ(counter.get_value(), MINES);
}

TEST(MinesCounter, Reset)
{
  mines_counter counter(MINES);

  counter.reset(MINES * 2);
  EXPECT_EQ(counter.get_value(), MINES * 2);
}

TEST(MinesCounter, Increment)
{
  mines_counter counter(MINES);

  ++counter;
  EXPECT_EQ(counter.get_value(), MINES + 1);
}

TEST(MinesCounter, Decrement)
{
  mines_counter counter(MINES);

  --counter;
  EXPECT_EQ(counter.get_value(), MINES - 1);
}

TEST(MinesCounter, ValueToString_ThreeDigits)
{
  mines_counter counter(119);

  static constexpr std::array str = { '1', '1', '9' };
  EXPECT_THAT(counter.value_to_str(), str);
}

TEST(MinesCounter, ValueToString_TwoDigits)
{
  mines_counter counter(42);

  static constexpr std::array str = { '0', '4', '2' };
  EXPECT_THAT(counter.value_to_str(), str);
}

TEST(MinesCounter, ValueToString_OneDigit)
{
  mines_counter counter(7);

  static constexpr std::array str = { '0', '0', '7' };
  EXPECT_THAT(counter.value_to_str(), str);
}

TEST(MinesCounter, ValueToString_TwoDigitsNegative)
{
  mines_counter counter(0);

  for (size_t i = 0; i < 42; ++i)
    --counter;

  static constexpr std::array str = { '-', '4', '2' };
  EXPECT_THAT(counter.value_to_str(), str);
}

TEST(MinesCounter, ValueToString_OneDigitNegative)
{
  mines_counter counter(0);

  for (size_t i = 0; i < 7; ++i)
    --counter;

  static constexpr std::array str = { '-', '0', '7' };
  EXPECT_THAT(counter.value_to_str(), str);
}

TEST(MinesCounter, ValueToString_Zero)
{
  mines_counter counter(0);

  static constexpr std::array str = { '0', '0', '0' };
  EXPECT_THAT(counter.value_to_str(), str);
}

TEST(MinesCounter, ValueToString_BigNumber)
{
  mines_counter counter(12345);

  static constexpr std::array str = { '0', '0', '0' };
  EXPECT_THAT(counter.value_to_str(), str);
}
