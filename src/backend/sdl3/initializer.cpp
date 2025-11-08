#include "immpp/initializer.hpp"
#include "SDL3/SDL_init.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "immpp/types.hpp"

namespace immpp {

// NOLINTNEXTLINE
opt_error Initializer::init() const noexcept {
 if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    return opt_error{error_codes::SDL_INIT};
  }

  if (!TTF_Init()) {
    return opt_error{error_codes::SDL_INIT};
  }

  return ds::null;
}

Initializer::~Initializer() noexcept {
  TTF_Quit();
  SDL_Quit();
}

} // namespace immpp
