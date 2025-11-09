#include "immpp/initializer.hpp"
#include "immpp/logger.hpp"
#include "immpp/size.hpp"
#include "immpp/types.hpp"
#include "immpp/window.hpp"
#include <array>

using namespace immpp;

namespace {

const rect<f32> grow_rectangle{0.0F, 0.0F, size::GROW_F32, size::GROW_F32};
const std::array<i32, 2> sizes{
  size::encode_fixed(100),
  size::encode_fixed(50),
};

void to_char(std::array<c8, 10>& string, i32 num) {
  i32 i = 0;
  while (num > 0) {
    string[i] = (num % 10) + '0';
    num /= 10;
    ++i;
  }
  for (i32 j = (i / 2) - 1; j > -1; --j) {
    std::swap(string[j], string[i - j - 1]);
  }
  string[i] = '\0';
}

void render_id(
    Window& window, i32& id, std::array<c8, 10>& string, rgba8 color
) {
  to_char(string, ++id);
  window.start_group();
  {
    window.add_group(grow_rectangle);
    window.fill_rectangle(color);

    window.add_group(grow_rectangle);
    if (window.text_button(string.data())) {
      logger::info("Pressed %d", id);
    }
  }
  window.end_group();
}

void render_group(
    Window& window, i32& id, std::array<c8, 10>& string, rgba8 color
) {
  window.start_column(sizes.data(), sizes.size());
  for (i32 i = 0; i < 2; ++i) {
    window.start_row(sizes.data(), sizes.size());
    {
      render_id(window, id, string, color);
      render_id(window, id, string, color);
    }
    window.end_row();
  }
  window.end_column();
}

} // namespace

i32 main() noexcept {
  Initializer initializer{};
  opt_error error = initializer.init();
  if (error) {
    logger::error("Initializer error: %d\n", *error);
    return -1;
  }

  {
    Window window{};
    error = window.init("Animation");
    if (error) {
      logger::error("Window error: %d\n", *error);
      return -1;
    }

    // Configuration
    error = window.set_font("../assets/fonts/PixeloidSans.ttf", 16);
    if (error) {
      logger::error("Font error: %d\n", *error);
      return -1;
    }

    i32 id = 0;
    std::array<c8, 10> string{};
    std::array<rgba8, 9> colors{
      rgba8{0x80, 0xff, 0x80, 0xff}, rgba8{0x00, 0xff, 0x00, 0xff},
      rgba8{0x00, 0xff, 0xff, 0xff}, rgba8{0xff, 0x00, 0x00, 0xff},
      rgba8{0xff, 0x00, 0xff, 0xff}, rgba8{0xff, 0xff, 0x00, 0xff},
      rgba8{0x80, 0x80, 0x80, 0xff}, rgba8{0xff, 0x80, 0x80, 0xff},
      rgba8{0x80, 0x80, 0xff, 0xff},
    };

    while (window.start()) {
      id = 0;
      window.set_anchor(Alignment::HORIZONTAL_LEFT | Alignment::VERTICAL_TOP);
      render_group(window, id, string, colors[0]);

      window.set_anchor(Alignment::HORIZONTAL_CENTER | Alignment::VERTICAL_TOP);
      render_group(window, id, string, colors[1]);

      window.set_anchor(Alignment::HORIZONTAL_RIGHT | Alignment::VERTICAL_TOP);
      render_group(window, id, string, colors[2]);

      window.set_anchor(
          Alignment::HORIZONTAL_LEFT | Alignment::VERTICAL_CENTER
      );
      render_group(window, id, string, colors[3]);

      window.set_anchor(
          Alignment::HORIZONTAL_CENTER | Alignment::VERTICAL_CENTER
      );
      render_group(window, id, string, colors[4]);

      window.set_anchor(
          Alignment::HORIZONTAL_RIGHT | Alignment::VERTICAL_CENTER
      );
      render_group(window, id, string, colors[5]);

      window.set_anchor(
          Alignment::HORIZONTAL_LEFT | Alignment::VERTICAL_BOTTOM
      );
      render_group(window, id, string, colors[6]);

      window.set_anchor(
          Alignment::HORIZONTAL_CENTER | Alignment::VERTICAL_BOTTOM
      );
      render_group(window, id, string, colors[7]);

      window.set_anchor(
          Alignment::HORIZONTAL_RIGHT | Alignment::VERTICAL_BOTTOM
      );
      render_group(window, id, string, colors[8]);

      window.end();
    }
  }

  return 0;
}
