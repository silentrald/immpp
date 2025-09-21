#include "./window.hpp"
#include "SDL3/SDL_init.h"
#include <cstdio>

int main() noexcept {
  // Initialization
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    printf("Unable to init SDL : %s", SDL_GetError());
    return -1;
  }

  {
    sdl_imm::Window window{};
    sdl_imm::opt_error error = window.init("Hello World");
    if (error) {
      // TODO:
      printf("Error %d\n", *error);
    }

    bool running = true;
    sdl_imm::rect<sdl_imm::f32> r1{10.0, 10.0, 10.0, 10.0};
    sdl_imm::rect<sdl_imm::f32> r2{30.0, 30.0, 10.0, 10.0};
    while (window.start()) {
      window.draw_rectangle(r1, {0xFF, 0x00, 0x00, 0xFF});
      window.fill_rectangle(r2, {0x00, 0xFF, 0x00, 0xFF});
      window.end();
    }
  }

  SDL_Quit();

  return 0;
}
