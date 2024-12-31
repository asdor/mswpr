#ifndef MSWPR_GAME_TIMER_HPP
#define MSWPR_GAME_TIMER_HPP

#include <array>
#include <chrono>
#include <cstdint>

namespace mswpr
{
  class game_timer
  {
  public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    game_timer();

    bool is_running() const;

    void start(const TimePoint& i_start_time);
    void update(const TimePoint& i_current_time);
    void stop(const TimePoint& i_stop_time);
    void reset();

    std::chrono::seconds get_elapsed_time() const;
    std::array<uint8_t, 3> extract_digits_from_seconds() const;

    static TimePoint now();

  private:
    TimePoint d_start_time;
    TimePoint d_current_time;
    bool d_is_running;
  };
}

#endif  // MSWPR_GAME_TIMER_HPP
