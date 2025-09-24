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

template <typename T> struct vec2 {
  T x;
  T y;
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

  bool contains(vec2<T> point) {
    return point.x >= this->x && point.x <= this->x + this->w &&
           point.y >= this->y && point.y <= this->y + this->h;
  }
};

struct rgba8 {
  u8 r;
  u8 g;
  u8 b;
  u8 a;
};

// Error
// NOLINTNEXTLINE
enum error_codes : i32 {
  OK = 0,

  SDL_INIT_ERROR,

  UNKNOWN, //
};

using error_code = i32;
template <typename T> using exp_error = ds::expected<T, error_code>;
using opt_error = ds::optional<error_code>;
const auto null = ds::null;

} // namespace immpp
