#ifndef IMMPP_TYPES
#define IMMPP_TYPES

#include "ds/expected.hpp"
#include "ds/optional.hpp"
#include <cstdint>

namespace immpp {

using c8 = char;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

// Error
// NOLINTNEXTLINE
enum error_codes : i32 {
  OK = 0,

  SDL_INIT,
  SDL_BAD_ALLOCATION,

  UNKNOWN, //
};

using error_code = i32;
template <typename T> using exp_error = ds::expected<T, error_code>;
using opt_error = ds::optional<error_code>;

template <typename T> struct vec2 {
  T x;
  T y;

  [[nodiscard]] vec2<T> operator+(vec2<T> rhs) const noexcept {
    return vec2<T>{.x = this->x + rhs.x, .y = this->y + rhs.y};
  }

  [[nodiscard]] vec2<T> operator-(vec2<T> rhs) const noexcept {
    return vec2<T>{.x = this->x - rhs.x, .y = this->y - rhs.y};
  }

  template <typename T2> [[nodiscard]] vec2<T2> to() const noexcept {
    return immpp::vec2<T2>{.x = (T2)this->x, .y = (T2)this->y};
  }
};

template <typename T> struct vec3 {
  T x;
  T y;
  T z;
};

template <typename T> struct rect {
  union {
    struct {
      T x;
      T y;
    };
    vec2<T> position;
  };
  union {
    struct {
      T w;
      T h;
    };
    vec2<T> size;
  };

  error_code copy(rect<T> other) noexcept {
    *this = other;
    return error_codes::OK;
  }

  bool contains(vec2<T> point) const noexcept {
    return point.x >= this->x && point.x <= this->x + this->w &&
           point.y >= this->y && point.y <= this->y + this->h;
  }
};

struct rgba8 {
  u8 r;
  u8 g;
  u8 b;
  u8 a;

  error_code copy(rgba8 other) noexcept {
    *this = other;
    return error_codes::OK;
  }
};

} // namespace immpp

#endif
