#include "./window.hpp"
#include "SDL3/SDL_init.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <cstdio>

using namespace immpp;

class Initializer {
public:
  Initializer() noexcept = default;
  Initializer(const Initializer&) = delete;
  Initializer(Initializer&&) = delete;
  Initializer& operator=(const Initializer&) = delete;
  Initializer& operator=(Initializer&&) = delete;

  // NOLINTNEXTLINE
  [[nodiscard]] opt_error init() const noexcept {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
      // TODO:
      return opt_error{error_codes::SDL_INIT_ERROR};
    }

    if (!TTF_Init()) {
      // TODO:
      return opt_error{error_codes::SDL_INIT_ERROR};
    }

    // TODO: Check this return
    return opt_error{null};
  }

  ~Initializer() noexcept {
    TTF_Quit();
    SDL_Quit();
  }
};

int main() noexcept {
  Initializer initializer{};
  opt_error error = initializer.init();
  if (error) {
    printf("Error %d\n", *error);
    return -1;
  }

  {
    Window window{};
    error = window.init("Hello World");
    if (error) {
      printf("Error %d\n", *error);
      return *error;
    }

    while (window.start_render()) {
      if (window.text_button(vec2<f32>{0.0F, 50.0F}, "Something")) {
        printf("UwU\n");
      }
      if (window.text_button(vec2<f32>{100.0F, 50.0F}, "Asdf")) {
        printf("UwU\n");
      }
      window.end_render();
    }
  }

  return 0;
}
