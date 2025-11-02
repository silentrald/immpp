#include "./logger.hpp"
#include <cstdarg>
#include <cstdio>
#include <ctime>

namespace immpp {

#ifdef __unix__
#include <bits/types/struct_timeval.h>
#include <sys/time.h>

using tv = timeval;
#endif

#ifdef _WIN32
#include <Windows.h>
#include <stdint.h>

typedef struct tv {
  i64 tv_sec;
  i64 tv_usec;
} tv;

const u64 EPOCH = 116444736000000000ULL;
void gettimeofday(struct tv* tp, struct timezone* tzp) {
  SYSTEMTIME system_time;
  FILETIME file_time;
  u64 time;

  GetSystemTime(&system_time);
  SystemTimeToFileTime(&system_time, &file_time);
  time = (u64)file_time.dwLowDateTime;
  time = (u64)file_time.dwHighDateTime << 32;

  tp->tv_sec = (i32)((time - EPOCH) / 10000000);
  tp->tv_usec = (i32)(system_time.wMilliseconds * 1000);
}
#endif

// === Consts === //
const char* const TEXT_WHITE = "\e[97m";
const char* const TEXT_BLACK = "\e[30m";
const char* const TEXT_RED = "\x1B[31m";
const char* const TEXT_GREEN = "\x1B[32m";
const char* const TEXT_YELLOW = "\x1B[33m";
const char* const TEXT_BLUE = "\x1B[34m";
const char* const TEXT_VIOLET = "\x1B[93m";
const char* const TEXT_MAGENTA = "\e[95m";
const char* const BG_RED = "\e[31m";
const char* const TEXT_BOLD = "\033[1m";
const char* const TEXT_NORMAL = "\033[0m";
const char* const RESET = "\x1B[0m";

const char* const FATAL_LABEL = "fatal";
const char* const ERROR_LABEL = "error";
const char* const WARN_LABEL = "warn";
const char* const INFO_LABEL = "info";
const char* const DEBUG_LABEL = "debug";

const char* const TIMESTAMP_FMT = "YYYY-MM-DDTHH:mm:ss.SSS";
const i32 TIMESTAMP_LENGTH = sizeof "YYYY-MM-DDTHH:mm:ss.SSS";
char timestamp[TIMESTAMP_LENGTH]; // NOLINT

namespace {

// NOLINTNEXTLINE
inline static LogLevel level = LogLevel::DEBUG;

// NOTE: No multi-threading is done, yet
/* std::mutex mutex{}; */

const char* get_timestamp() noexcept {
  tv current = {};
  gettimeofday(&current, nullptr);

  static_cast<void>(strftime(
      timestamp, TIMESTAMP_LENGTH, "%Y-%m-%dT%H:%M:%S",
      localtime(&current.tv_sec) // NOLINT
  ));
  timestamp[sizeof "YYYY-MM-DDTHH:mm:ss"] = '\0';

  // NOLINTNEXTLINE
  sprintf(timestamp, "%s.%03ld", timestamp, current.tv_usec / 1000);
  return timestamp;
}

// === Utils === //
inline void print_header(
    const char* level, const char* level_fg, const char* level_bg
) noexcept {
  printf(
      "%s %s%s%s%s:%s ", get_timestamp(), TEXT_BOLD, level_fg, level_bg, level,
      RESET
  );
}

inline void print_header(const char* level, const char* level_fg) noexcept {
  printf("%s %s%s%s:%s ", get_timestamp(), TEXT_BOLD, level_fg, level, RESET);
}

} // namespace

void logger::set_level(LogLevel new_level) noexcept {
  level = new_level;
}

// === Fatal === //
// NOLINTNEXTLINE
void logger::fatal(const c8* message, ...) noexcept {
  if (level < LogLevel::FATAL) {
    return;
  }

  print_header(FATAL_LABEL, TEXT_BLACK, BG_RED);

  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("\n");
}

// === Error === //
// NOLINTNEXTLINE
void logger::error(const c8* message, ...) noexcept {
  if (level < LogLevel::ERROR) { // ERROR is defined in Windows
    return;
  }

  print_header(ERROR_LABEL, TEXT_RED);

  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("\n");
}

// === WARN === //
// NOLINTNEXTLINE
void logger::warn(const c8* message, ...) noexcept {
  if (level < LogLevel::WARN) {
    return;
  }

  print_header(WARN_LABEL, TEXT_YELLOW);

  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("\n");
}

// === INFO === //
// NOLINTNEXTLINE
void logger::info(const c8* message, ...) noexcept {
  if (level < LogLevel::INFO) {
    return;
  }

  print_header(INFO_LABEL, TEXT_GREEN);

  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("\n");
}

// === DEBUG === //
// NOLINTNEXTLINE
void logger::debug(const c8* message, ...) noexcept {
  if (level < LogLevel::DEBUG) {
    return;
  }

  print_header(DEBUG_LABEL, TEXT_VIOLET);

  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("\n");
}

} // namespace immpp
