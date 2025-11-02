#ifndef IMMPP_SIZE_HPP
#define IMMPP_SIZE_HPP

#include "immpp/types.hpp"

namespace immpp::size {

[[nodiscard]] i32 encode_fixed(i32 size) noexcept;
[[nodiscard]] i32 decode_fixed(i32 encoded) noexcept;
[[nodiscard]] i32 encode_grow(i32 part) noexcept;
[[nodiscard]] i32 decode_grow(i32 encoded) noexcept;
[[nodiscard]] i32 encode_fit() noexcept;
[[nodiscard]] bool is_fit(i32 encoded) noexcept;
[[nodiscard]] bool is_grow(i32 encoded) noexcept;

} // namespace immpp::size

#endif
