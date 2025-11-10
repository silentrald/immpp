#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "ds/vector.hpp"
#include "immpp/size.hpp"
#include "immpp/types.hpp"

namespace immpp {

struct Theme {
  rgba8 foreground_color = {0x00, 0x00, 0x00, 0xff}; // White
  rgba8 background_color = {0xff, 0xff, 0xff, 0xff}; // Black
  vec2<f32> padding = {4.0F, 4.0F};
  vec2<f32> margin = {4.0F, 4.0F};
};

enum class MouseState : i8 {
  UP = 0x00,
  PRESSED = 0x01,
  DOWN = 0x02,
  RELEASED = 0x03,
};

struct Input {
  struct Mouse {
    vec2<f32> position{};
    vec2<f32> scroll{};

    struct Click {
      vec2<f32> left_position{};
      vec2<f32> right_position{};
      vec2<f32> middle_position{};
    } click;

    MouseState left = MouseState::UP;
    MouseState right = MouseState::UP;
    MouseState middle = MouseState::UP;
  } mouse;

  struct Keyboard {
    ds::vector<u32> keys{}; // todo fixed vector/array
  } keyboard;
};

enum Alignment : u8 {
  // Horizontal
  HORIZONTAL_LEFT = 0x00,
  HORIZONTAL_CENTER = 0x01,
  HORIZONTAL_RIGHT = 0x02,
  HORIZONTAL_MASK = 0x03,

  // Vertical
  VERTICAL_TOP = 0x00,
  VERTICAL_CENTER = 0x10,
  VERTICAL_BOTTOM = 0x20,
  VERTICAL_MASK = 0x30,
};

enum class Widget : u8 {
  NONE = 0,
  ROW,
  COLUMN,
  GROUP,
};

struct State {
  vec2<f32> window_size{640.0F, 480.0F};
  ds::vector<rect<f32>> widget_sizes{};
  ds::vector<Widget> widgets{};
  rect<f32> limits{};

  u64 time = 0;
  u32 seconds_per_frame = 1000 / 60;
  // u32 FPS = 60;
  u8 alignments = HORIZONTAL_LEFT | VERTICAL_TOP;
  bool running = true;
};

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

  // === Configuration === //

  void set_fps(u32 FPS) noexcept;
  [[nodiscard]] opt_error set_font(const c8* path, i32 size) noexcept;
  void set_window_size(vec2<i32> size) noexcept;

  // === Main Loop === //

  [[nodiscard]] bool start() noexcept;
  void end() noexcept;
  void quit() noexcept;

  // === Layouts === //

  // Check on Alignment enum for values
  void set_anchor(u8 alignments) noexcept;

  void start_row(const i32* widths, i32 widths_size) noexcept;
  void start_row(const ds::vector<i32>& widths) noexcept;
  void end_row() noexcept;

  void start_column(const i32* heights, i32 heights_size) noexcept;
  void start_column(const ds::vector<i32>& heights) noexcept;
  void end_column() noexcept;

  void start_group(vec2<i32> size = {size::GROW_I32, size::GROW_I32}) noexcept;
  void add_group(const rect<f32>& rectangle) noexcept;
  void end_group() noexcept;

  // === Widgets === //

  void text(const c8* string) noexcept;
  [[nodiscard]] bool text_button(const c8* text) noexcept;
  void image(const c8* path) noexcept;
  [[nodiscard]] bool image_button(const c8* path) noexcept;
  void rectangle(rgba8 color) noexcept;
  void fill_rectangle(rgba8 color) noexcept;

private:
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  TTF_Font* font = nullptr;

  Theme theme{};
  Input input{};
  State state{};
};

} // namespace immpp
