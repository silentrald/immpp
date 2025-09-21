#include "./types.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

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

  // TODO: For testing return
  [[nodiscard]] bool start() noexcept;
  void end() noexcept;

  // Drawing stuff
  void draw_rectangle(const rect<f32>& dimensions, rgba8 color) noexcept;
  void fill_rectangle(const rect<f32>& dimensions, rgba8 color) noexcept;

private:
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
};

} // namespace sdl_imm
