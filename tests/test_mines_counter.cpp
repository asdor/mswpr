#include "core/mines_counter.hpp"

#include <gtest/gtest.h>

using namespace mswpr;

namespace
{
  constexpr int MINES = 10;
}

TEST(MinesCounter, Constructor)
{
  const mines_counter counter(MINES);

  EXPECT_EQ(counter.get_value(), MINES);
}

TEST(MinesCounter, Reset)
{
  static constexpr auto COUNTER_VALUE = static_cast<size_t>(2) * MINES;
  mines_counter counter(MINES);
  counter.reset(COUNTER_VALUE);
  EXPECT_EQ(counter.get_value(), COUNTER_VALUE);
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
  const mines_counter counter(119);

  static constexpr std::array DIGITS = { '1', '1', '9' };
  EXPECT_EQ(counter.value_to_str(), DIGITS);
}

TEST(MinesCounter, ValueToString_TwoDigits)
{
  const mines_counter counter(42);

  static constexpr std::array DIGITS = { '0', '4', '2' };
  EXPECT_EQ(counter.value_to_str(), DIGITS);
}

TEST(MinesCounter, ValueToString_OneDigit)
{
  const mines_counter counter(7);

  static constexpr std::array DIGITS = { '0', '0', '7' };
  EXPECT_EQ(counter.value_to_str(), DIGITS);
}

TEST(MinesCounter, ValueToString_TwoDigitsNegative)
{
  mines_counter counter(0);

  for (size_t i = 0; i < 42; ++i)
    --counter;

  static constexpr std::array DIGITS = { '-', '4', '2' };
  EXPECT_EQ(counter.value_to_str(), DIGITS);
}

TEST(MinesCounter, ValueToString_OneDigitNegative)
{
  mines_counter counter(0);

  for (size_t i = 0; i < 7; ++i)
    --counter;

  static constexpr std::array DIGITS = { '-', '0', '7' };
  EXPECT_EQ(counter.value_to_str(), DIGITS);
}

TEST(MinesCounter, ValueToString_Zero)
{
  const mines_counter counter(0);

  static constexpr std::array DIGITS = { '0', '0', '0' };
  EXPECT_EQ(counter.value_to_str(), DIGITS);
}

TEST(MinesCounter, ValueToString_BigNumber)
{
  const mines_counter counter(12345);

  static constexpr std::array DIGITS = { '0', '0', '0' };
  EXPECT_EQ(counter.value_to_str(), DIGITS);
}
