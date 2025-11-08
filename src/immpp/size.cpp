#include "./size.hpp"
#include "immpp/types.hpp"
#include <cassert>

namespace immpp {

i32 size::encode_fixed(i32 size) noexcept {
  assert(!(size & GROW_I32));
  return size;
}

i32 size::decode_fixed(i32 encoded) noexcept {
  return encoded;
}

i32 size::encode_grow(i32 part) noexcept {
  return GROW_I32 | part;
}

i32 size::decode_grow(i32 encoded) noexcept {
  return ~GROW_I32 & encoded;
}

i32 size::encode_fit() noexcept {
  return FIT_I32;
}

bool size::is_fit(i32 encoded) noexcept {
  return FIT_I32 == encoded;
}

bool size::is_grow(i32 encoded) noexcept {
  return GROW_I32 & encoded;
}

bool size::is_type(f32 size) noexcept {
  return (*(i32*)&size) & 0x8000'0000;
}

bool size::is_fit(immpp::f32 size) noexcept {
  return ((*(i32*)&size) & 0xC000'0000) == 0xC000'0000;
}

bool size::is_stretch(immpp::f32 size) noexcept {
  return ((*(i32*)&size) & 0xA000'0000) == 0xA000'0000;
}



} // namespace immpp
