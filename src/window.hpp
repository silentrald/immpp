#include "./types.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "ds/vector.hpp"

namespace sdl_imm {

class Window {
public:
  Window() noexcept = default;
  Window(Window&& other) noexcept;
  Window& operator=(Window&& rhs) noexcept;

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  /**
   * Possible errors:
   * - SDL_INIT_ERROR
   **/
  [[nodiscard]] opt_error init(const c8* title) noexcept;
  ~Window() noexcept;

  // Drawing stuff

  [[nodiscard]] bool start_render() noexcept;
  void end_render() noexcept;

  // === Widgets === //

  [[nodiscard]] bool
  text_button(rect<f32> dimensions, const c8* text) noexcept;
  [[nodiscard]] bool
  text_button(vec2<f32> position, const c8* text) noexcept;
  void rectangle(rect<f32> dimensions, rgba8 color) noexcept;
  void fill_rectangle(rect<f32> dimensions, rgba8 color) noexcept;

private:
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  TTF_Font* font = nullptr;

  // === User inputs === //

  vec2<f32> mouse_position{};
  vec2<f32> mouse_scroll{};
  bool mouse_left = false;
  bool mouse_right = false;
  bool mouse_middle = false;
  ds::vector<u32> keys{}; // todo fixed vector/array

  // === Theming === //

  rgba8 foreground_color = {0x00, 0x00, 0x00, 0xff}; // White
  rgba8 background_color = {0xff, 0xff, 0xff, 0xff}; // Black
  vec2<f32> padding = {4.0F, 4.0F};
};

} // namespace sdl_imm
