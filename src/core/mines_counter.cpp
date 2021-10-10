#include "core/mines_counter.hpp"

#include <charconv>
#include <cmath>
#include <system_error>

namespace
{
  size_t get_number_ranks(int counter)
  {
    static constexpr std::array<size_t, 3> size_table = { 9, 99, 999 };

    size_t rank = 0;

    for (size_t x : size_table)
    {
      if (counter <= x)
        return rank + 1;

      ++rank;
    }

    return 3;
  }
}

mswpr::mines_counter::mines_counter(size_t counter) : counter_(static_cast<int>(counter))
{
}

int mswpr::mines_counter::get_value() const
{
  return counter_;
}

void mswpr::mines_counter::reset(size_t counter)
{
  counter_ = static_cast<int>(counter);
}

mswpr::mines_counter& mswpr::mines_counter::operator++()
{
  ++counter_;
  return *this;
}

mswpr::mines_counter& mswpr::mines_counter::operator--()
{
  --counter_;
  return *this;
}

std::array<char, 3> mswpr::mines_counter::value_to_str() const
{
  std::array<char, 3> str = { '0', '0', '0' };

  int counter = counter_;
  if (counter_ < 0)
  {
    str[0] = '-';
    counter = -counter;
  }

  const size_t rank = get_number_ranks(counter);

  std::to_chars(str.data() + (3 - rank), str.data() + str.size(), counter);
  return str;
}
