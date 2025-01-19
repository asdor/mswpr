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
    using clock_t = std::chrono::steady_clock;
    using time_point_t = clock_t::time_point;

    game_timer();

    bool is_running() const;

    void start(const time_point_t& i_start_time);
    void update(const time_point_t& i_current_time);
    void stop(const time_point_t& i_stop_time);
    void reset();

    std::chrono::seconds get_elapsed_time() const;
    std::array<uint8_t, 3> extract_digits_from_seconds() const;

    static time_point_t now();

  private:
    time_point_t d_start_time;
    time_point_t d_current_time;
    bool d_is_running;
  };
}  // namespace mswpr

#endif  // MSWPR_GAME_TIMER_HPP
