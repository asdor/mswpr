#include "core/game_timer.hpp"

#include <ranges>

mswpr::game_timer::game_timer() : d_is_running(false)
{
}

bool mswpr::game_timer::is_running() const
{
  return d_is_running;
}

void mswpr::game_timer::start(const time_point_t& i_start_time)
{
  if (d_is_running)
    return;

  d_start_time = i_start_time;
  d_current_time = d_start_time;
  d_is_running = true;
}

void mswpr::game_timer::update(const time_point_t& i_current_time)
{
  if (d_is_running)
    d_current_time = i_current_time;
}

void mswpr::game_timer::stop(const time_point_t& i_stop_time)
{
  if (!d_is_running)
    return;

  d_current_time = i_stop_time;
  d_is_running = false;
}

void mswpr::game_timer::reset()
{
  d_start_time = {};
  d_current_time = {};
  d_is_running = false;
}

std::chrono::seconds mswpr::game_timer::get_elapsed_time() const
{
  const auto diff = d_current_time - d_start_time;
  return std::chrono::duration_cast<std::chrono::seconds>(diff);
}

mswpr::game_timer::time_point_t mswpr::game_timer::now()
{
  return clock_t::now();
}

std::array<uint8_t, 3> mswpr::game_timer::extract_digits_from_seconds() const
{
  static constexpr auto MAX_DIGITS = static_cast<time_point_t::rep>(999);
  std::array<uint8_t, 3> arr = { 0, 0, 0 };
  auto seconds = std::min(get_elapsed_time().count(), MAX_DIGITS);

  for (auto& x : std::ranges::reverse_view(arr))
  {
    x = seconds % 10;
    seconds /= 10;
  }

  return arr;
}
