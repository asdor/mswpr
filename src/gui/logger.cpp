#include "gui/logger.hpp"

#include <SDL.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <filesystem>

namespace
{
  constexpr auto MSWPR_LOGGER_NAME = "mswpr";

  std::shared_ptr<spdlog::logger> initialize_logger()
  {
    const std::filesystem::path log_file = SDL_GetBasePath();
    auto logger = spdlog::basic_logger_st(MSWPR_LOGGER_NAME, log_file / "sdl2_minesweeper.log");
    spdlog::set_level(spdlog::level::debug);
    return logger;
  }
}

std::shared_ptr<spdlog::logger> mswpr::get_mswpr_logger()
{
  auto logger = spdlog::get(MSWPR_LOGGER_NAME);
  if (!logger)
    return ::initialize_logger();
  return logger;
}
