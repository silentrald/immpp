#include "SDL3/SDL_init.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "immpp/logger.hpp"
#include "immpp/size.hpp"
#include "immpp/window.hpp"

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
      return opt_error{error_codes::SDL_INIT};
    }

    if (!TTF_Init()) {
      // TODO:
      return opt_error{error_codes::SDL_INIT};
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
    logger::error("Error %d", *error);
    return -1;
  }

  {
    Window window{};
    error = window.init("Hello World");
    if (error) {
      logger::error("Error %d", *error);
      return *error;
    }

    const i32 widths[]{
      immpp::size::encode_fixed(100), immpp::size::encode_grow(1)
    };
    const i32 heights[]{
      immpp::size::encode_fixed(100), immpp::size::encode_grow(1)
    };
    while (window.start()) {
      window.start_row(widths, 2);
      {
        window.start_column(heights, 2);
        {
          if (window.text_button("Quit")) {
            logger::info("Quiting...");
            window.quit();
          }
          window.text("Sidepanel");
        }
        window.end_column();

        window.start_group();
        window.set_group_offset({-5.0F, 0.0F});
        window.text("Hi");

        window.set_group_offset({100.0F, 100.0F});
        window.text("World");

        window.end_group();
      }
      window.end_row();

      window.end();
    }
  }

  logger::info("QUIT");

  return 0;
}
