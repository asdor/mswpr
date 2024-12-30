#include "core/game_timer.hpp"

mswpr::game_timer::game_timer() : d_is_running(false)
{
}

void mswpr::game_timer::start(const TimePoint& i_start_time)
{
  if (d_is_running)
    return;

  d_start_time = i_start_time;
  d_current_time = d_start_time;
  d_is_running = true;
}

void mswpr::game_timer::update(const TimePoint& i_current_time)
{
  if (d_is_running)
    d_current_time = i_current_time;
}

void mswpr::game_timer::stop(const TimePoint& i_stop_time)
{
  if (!d_is_running)
    return;

  d_current_time = i_stop_time;
  d_is_running = false;
}

std::chrono::seconds mswpr::game_timer::get_elapsed_time() const
{
  const auto diff = d_current_time - d_start_time;
  return std::chrono::duration_cast<std::chrono::seconds>(diff);
}

mswpr::game_timer::TimePoint mswpr::game_timer::now()
{
  return Clock::now();
}

std::array<uint8_t, 3> mswpr::game_timer::as_digit_array() const
{
  std::array<uint8_t, 3> arr = { 0, 0, 0 };
  auto seconds = get_elapsed_time().count();
  for (auto it = arr.rbegin(); it != arr.rend(); ++it)
  {
    *it = seconds % 10;
    seconds /= 10;
  }

  return arr;
}
