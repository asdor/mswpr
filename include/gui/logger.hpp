#ifndef MSWPR_LOGGER_HPP
#define MSWPR_LOGGER_HPP

#include <spdlog/fwd.h>

#include <memory>

namespace mswpr
{
  std::shared_ptr<spdlog::logger> get_mswpr_logger();
}

#endif  // MSWPR_LOGGER_HPP