#ifndef IMMPP_INITIALIZER_HPP
#define IMMPP_INITIALIZER_HPP

#include "immpp/types.hpp"

namespace immpp {

class Initializer {
public:
  Initializer() noexcept = default;
  Initializer(const Initializer&) = delete;
  Initializer(Initializer&&) = delete;
  Initializer& operator=(const Initializer&) = delete;
  Initializer& operator=(Initializer&&) = delete;

  // NOLINTNEXTLINE
  [[nodiscard]] opt_error init() noexcept;
  ~Initializer() noexcept;

private:
  bool initialized = false;
};

} // namespace immpp

#endif
