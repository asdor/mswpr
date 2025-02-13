#ifndef MSWPR_MINES_COUNTER_HPP
#define MSWPR_MINES_COUNTER_HPP

#include <array>

namespace mswpr
{
  class mines_counter
  {
  public:
    mines_counter(std::size_t counter);

    int get_value() const;
    void reset(std::size_t counter);
    std::array<char, 3> value_to_str() const;

    mines_counter& operator++();
    mines_counter& operator--();

  private:
    int d_counter;
  };
}  // namespace mswpr

#endif  // MSWPR_MINES_COUNTER_HPP
