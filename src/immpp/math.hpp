#ifndef IMMPP_MATH_HPP
#define IMMPP_MATH_HPP

#include "immpp/types.hpp"

namespace immpp::math {

[[nodiscard]] bool compare(f32 f1, f32 f2) noexcept;
[[nodiscard]] bool compare(f64 d1, f64 d2) noexcept;

} // namespace immpp::math

#endif
