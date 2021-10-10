#ifndef MSWPR_MINES_COUNTER_HPP
#define MSWPR_MINES_COUNTER_HPP

#include <array>

namespace mswpr
{
  class mines_counter
  {
  public:
    mines_counter(int counter);

    int get_value() const;
    void reset(int counter);
    std::array<char, 3> value_to_str() const;

    mines_counter& operator++();
    mines_counter& operator--();

  private:
    int counter_;
  };
}

#endif  // MSWPR_MINES_COUNTER_HPP
