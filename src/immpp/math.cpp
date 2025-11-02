#include "./math.hpp"
#include <cmath>

namespace immpp {

const f32 F32_EPSILON = 0.000001F;
const f64 F64_EPSILON = 0.000000'000001;

bool math::compare(f32 f1, f32 f2) noexcept {
  return std::fabs(f1 - f2) < F32_EPSILON;
}

bool math::compare(f64 d1, f64 d2) noexcept {
  return std::fabs(d1 - d2) < F64_EPSILON;
}

} // namespace immpp
