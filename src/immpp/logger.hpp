#ifndef IMMPP_LOGGER_HPP
#define IMMPP_LOGGER_HPP

#include "immpp/types.hpp"

namespace immpp {

enum LogLevel : u8 {
  SILENT = 0,
  FATAL = 1,
  ERROR = 2,
  WARN = 3,
  INFO = 4,
  DEBUG = 5,
};

namespace logger {

void set_level(LogLevel new_level) noexcept;
void fatal(const c8* message, ...) noexcept;
void error(const c8* message, ...) noexcept;
void warn(const c8* message, ...) noexcept;
void info(const c8* message, ...) noexcept;
void debug(const c8* message, ...) noexcept;

} // namespace logger

} // namespace immpp

#endif
