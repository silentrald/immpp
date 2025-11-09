#ifndef IMMPP_SIZE_HPP
#define IMMPP_SIZE_HPP

#include "immpp/types.hpp"

namespace immpp::size {

// === Int Sizes === //

const i32 FIT_I32 = 0;
const i32 GROW_I32 = 0x8000'0000;

[[nodiscard]] i32 encode_fixed(i32 size) noexcept;
[[nodiscard]] i32 decode_fixed(i32 encoded) noexcept;
[[nodiscard]] i32 encode_grow(i32 part) noexcept;
[[nodiscard]] i32 decode_grow(i32 encoded) noexcept;
[[nodiscard]] i32 encode_fit() noexcept;
[[nodiscard]] bool is_fit(i32 encoded) noexcept;
[[nodiscard]] bool is_grow(i32 encoded) noexcept;

// === Float Sizes === //

union hex_to_float {
  immpp::u32 _;
  immpp::f32 f;
};

const f32 FIT_F32 = hex_to_float{0xC000'0000}.f;
const f32 GROW_F32 = hex_to_float{0xA000'0000}.f;

[[nodiscard]] bool is_type(immpp::f32 size) noexcept;
[[nodiscard]] bool is_fit(immpp::f32 size) noexcept;
[[nodiscard]] bool is_stretch(immpp::f32 size) noexcept;

} // namespace immpp::size

#endif
