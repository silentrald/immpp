#include "immpp/initializer.hpp"
#include "immpp/logger.hpp"
#include "immpp/size.hpp"
#include "immpp/types.hpp"
#include "immpp/window.hpp"
#include <array>

using namespace immpp;

int main() noexcept {
  Initializer initializer{};
  opt_error error = initializer.init();
  if (error) {
    logger::error("Error %d\n", *error);
    return -1;
  }

  {
    Window window{};
    error = window.init("Animation");
    if (error) {
      logger::error("Error %d\n", *error);
      return -1;
    }

    const std::array<i32, 2> heights{
      size::encode_grow(1), size::encode_fixed(100)
    };
    const std::array<i32, 3> widths{
      size::encode_fixed(32), size::encode_grow(1), size::encode_fixed(100)
    };
    while (window.start()) {
      window.start_column(heights.data(), heights.size());
      {
        window.start_row(widths.data(), widths.size());
        {
          window.start_group();
          {
            for (i32 i = 0; i < 5; ++i) {
              window.add_group({0.0F, 32.0F * i, 32.0F, 32.0F});
              if (window.image_button("../assets/images/sample.png")) {
                logger::info("Tool %d pressed", i);
              }
            }

            window.add_group({0.0F, 32.0F * 5, size::GROW_F32, size::GROW_F32});
            if (window.image_button("../assets/images/sample.png")) {
              logger::info("Tool %d pressed", 5);
            }
          }
          window.end_group();

          window.start_group();
          {
            window.add_group({0.0F, 0.0F, size::GROW_F32, size::GROW_F32});
            window.fill_rectangle({0xff, 0x00, 0x00, 0xff});
            window.add_group({0.0F, 0.0F, size::GROW_F32, size::GROW_F32});
            if (window.text_button("Canvas")) {
              logger::info("Canvas pressed");
            }
          }
          window.end_group();

          window.text("Layers");
        }
        window.end_row();

        window.text("Animation");
      }
      window.end_row();

      window.end();
    }
  }

  logger::info("QUIT");

  return 0;
}
