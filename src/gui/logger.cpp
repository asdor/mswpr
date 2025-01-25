#include "gui/logger.hpp"

#include <SDL.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <filesystem>

namespace
{
  constexpr auto LOGGER_NAMES = { "engine", "game" };
}

mswpr::logging_system::logging_system()
{
  const std::filesystem::path path_to_binary = SDL_GetBasePath();
  const std::filesystem::path log_file = path_to_binary / "sdl2_minesweeper.log";
  for (const auto logger_name : LOGGER_NAMES)
  {
    auto logger = spdlog::basic_logger_st(logger_name, log_file.string());
    logger->set_level(spdlog::level::debug);
  }
}
