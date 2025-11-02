#include "./size.hpp"
#include "immpp/types.hpp"
#include <cassert>

namespace immpp {

const i32 FIT_CODE = 0;
const i32 GROW_FLAG = 0x8000'0000;

i32 size::encode_fixed(i32 size) noexcept {
  assert(!(size & GROW_FLAG));
  return size;
}

i32 size::decode_fixed(i32 encoded) noexcept {
  return encoded;
}

i32 size::encode_grow(i32 part) noexcept {
  return GROW_FLAG | part;
}

i32 size::decode_grow(i32 encoded) noexcept {
  return ~GROW_FLAG & encoded;
}

i32 size::encode_fit() noexcept {
  return FIT_CODE;
}

bool size::is_fit(i32 encoded) noexcept {
  return FIT_CODE == encoded;
}

bool size::is_grow(i32 encoded) noexcept {
  return GROW_FLAG & encoded;
}

} // namespace immpp
