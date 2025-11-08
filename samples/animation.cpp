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
    logger::error("Error %d", *error);
    return -1;
  }

  {
    Window window{};
    error = window.init("Animation");
    if (error) {
      logger::error("Error %d", *error);
      return *error;
    }

    const std::array<i32, 2> heights{
      size::encode_grow(1), size::encode_fixed(100)
    };
    const std::array<i32, 3> widths{
      size::encode_fixed(50), size::encode_grow(1), size::encode_fixed(100)
    };
    while (window.start()) {
      window.start_column(heights.data(), heights.size());
      {
        window.start_row(widths.data(), widths.size());
        {
          window.text("Tools");

          window.start_group();
          {
            window.add_group({0.0F, 0.0F, size::GROW_F32, size::GROW_F32});
            window.fill_rectangle({0xff, 0x00, 0x00, 0xff});
            window.add_group({0.0F, 0.0F, size::GROW_F32, size::GROW_F32});
            window.text("Canvas");
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
