#include "immpp/window.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "ds/types.hpp"
#include "ds/vector.hpp"
#include "immpp/logger.hpp"
#include "immpp/size.hpp"
#include "immpp/types.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>

namespace {

// TODO: can add alignment logic here
[[nodiscard]] SDL_FRect calculate_widget_rectangle(
  immpp::rect<immpp::f32> area,
  immpp::vec2<immpp::f32> fit_size,
  immpp::vec2<immpp::f32> max_size
) noexcept {
  SDL_FRect output{
    // std::round to avoid blurry text
    .w = fit_size.x,
    .h = fit_size.y
  };

  immpp::f32 tmp = 0.0F;
  if (immpp::size::is_type(area.w)) {
    tmp = immpp::size::is_fit(area.w) ? fit_size.x : max_size.x;
  } else {
    tmp = area.w;
  }
  // std::round to avoid blurry text
  output.x = std::round(area.x + ((tmp - fit_size.x) / 2.0F));

  if (immpp::size::is_type(area.h)) {
    tmp = immpp::size::is_fit(area.h) ? fit_size.y : max_size.y;
  } else {
    tmp = area.h;
  }
  // std::round to avoid blurry text
  output.y = std::round(area.y + ((tmp - fit_size.y) / 2.0F));

  return output;
}

inline void set_color(SDL_Renderer* renderer, immpp::rgba8 color) noexcept {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

// NOTE: Can create a static vector implementation with ds
[[nodiscard]] inline immpp::i32
key_index(const ds::vector<immpp::u32>& keys, immpp::u32 key) noexcept {
  for (immpp::i32 i = 0; i < keys.get_size(); ++i) {
    if (keys[i] == key) {
      return i;
    }
  }

  return -1;
}

[[nodiscard]] inline SDL_Texture* create_text_texture(
    SDL_Renderer* renderer, TTF_Font* font, const immpp::c8* text,
    immpp::i32 text_length, immpp::rgba8 color
) noexcept {
  // Draw the text
  SDL_Surface* surface =
      TTF_RenderText_Solid(font, text, text_length, *(SDL_Color*)&color);
  if (surface == nullptr) {
    return nullptr;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  return texture;
}

inline void update_mouse_state(immpp::MouseState& mouse) {
  if (mouse == immpp::MouseState::PRESSED) {
    mouse = immpp::MouseState::DOWN;
  } else if (mouse == immpp::MouseState::RELEASED) {
    mouse = immpp::MouseState::UP;
  }
}

} // namespace

namespace immpp {

Window::Window(Window&& other) noexcept
    : window(other.window), renderer(other.renderer) {
  other.window = nullptr;
  other.renderer = nullptr;
}

Window& Window::operator=(Window&& rhs) noexcept {
  if (this == &rhs) {
    return *this;
  }

  this->window = rhs.window;
  this->renderer = rhs.renderer;
  rhs.window = nullptr;
  rhs.renderer = nullptr;

  return *this;
}

opt_error Window::init(const c8* title) noexcept {
  this->state.window_size = {640.0F, 480.0F};
  this->window = SDL_CreateWindow(title, 640, 480, SDL_WINDOW_RESIZABLE);
  if (this->window == nullptr) {
    return opt_error{error_codes::SDL_INIT};
  }

  this->renderer = SDL_CreateRenderer(this->window, nullptr);
  if (this->renderer == nullptr) {
    return opt_error{error_codes::SDL_INIT};
  }

  this->font = TTF_OpenFont("../assets/fonts/PixeloidSans.ttf", 16);
  if (this->font == nullptr) {
    return opt_error{error_codes::SDL_INIT};
  }

  auto error = this->state.widget_sizes.reserve(4);
  if (ds::is_error(error)) {
    return opt_error{error_codes::SDL_BAD_ALLOCATION};
  }

  return ds::null;
}

Window::~Window() noexcept {
  if (this->font != nullptr) {
    TTF_CloseFont(this->font);
    this->font = nullptr;
  }

  if (this->renderer != nullptr) {
    SDL_DestroyRenderer(this->renderer);
    this->renderer = nullptr;
  }

  if (this->window != nullptr) {
    SDL_DestroyWindow(this->window);
    this->window = nullptr;
  }
}

// === Drawing Stuff === //

// NOLINTNEXTLINE
bool Window::start() noexcept {
  if (!state.running) {
    return false;
  }

  SDL_Event event{};
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      return false;

    case SDL_EVENT_MOUSE_MOTION:
      this->input.mouse.position.x = event.motion.x;
      this->input.mouse.position.y = event.motion.y;
      break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      if (event.button.button == SDL_BUTTON_LEFT) {
        this->input.mouse.left = MouseState::PRESSED;
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        this->input.mouse.right = MouseState::PRESSED;
      } else if (event.button.button == SDL_BUTTON_MIDDLE) {
        this->input.mouse.middle = MouseState::PRESSED;
      }
      break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
      if (event.button.button == SDL_BUTTON_LEFT) {
        this->input.mouse.left = MouseState::RELEASED;
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        this->input.mouse.right = MouseState::RELEASED;
      } else if (event.button.button == SDL_BUTTON_MIDDLE) {
        this->input.mouse.middle = MouseState::RELEASED;
      }
      break;

    case SDL_EVENT_KEY_DOWN:
      if (this->input.keyboard.keys.get_size() >= 10) {
        break;
      }

      if (key_index(this->input.keyboard.keys, event.key.key) == -1) {
        (void)this->input.keyboard.keys.push(event.key.key);
      }
      break;

    case SDL_EVENT_KEY_UP:
      if (this->input.keyboard.keys.is_empty()) {
        break;
      }

      {
        i32 index = key_index(this->input.keyboard.keys, event.key.key);
        if (index > -1) {
          this->input.keyboard.keys.remove(index);
        }
      }
      break;

    case SDL_EVENT_WINDOW_RESIZED:
      this->state.window_size.x = event.window.data1;
      this->state.window_size.y = event.window.data2;
      break;

    default:
      break;
    }
  }

  // Update variable values
  this->state.widget_sizes.clear();
  static_cast<void>(this->state.widget_sizes.push(
      {.x = 0.0F, .y = 0.0F, .size = this->state.window_size}
  ));

  // Clear screen
  SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(this->renderer);

  return true;
}

void Window::end() noexcept {
  SDL_RenderPresent(this->renderer);

  update_mouse_state(this->input.mouse.left);
  update_mouse_state(this->input.mouse.right);
  update_mouse_state(this->input.mouse.middle);

  // TODO: Add frame limiter here
}

void Window::quit() noexcept {
  this->state.running = false;
}

// === Layouts === //

void Window::start_row(const i32* widths, i32 widths_size) noexcept {
  auto rectangle = this->state.widget_sizes.pop();

  i32 width = 0;
  f32 dynamic_width = rectangle.w;
  i32 parts = 0;
  for (i32 i = 0; i < widths_size; ++i) {
    width = widths[i];
    if (size::is_grow(width)) {
      parts += size::decode_grow(width);
    } else {
      dynamic_width -= size::decode_fixed(width);
    }
  }

  dynamic_width = std::max(dynamic_width, 0.0F);

  if (parts > 0) {
    dynamic_width /= parts;
  } else {
    rectangle.w -= dynamic_width;
  }

  rect<f32> new_rect{.y = rectangle.y, .h = rectangle.h};
  error_code error = error_codes::OK;
  for (i32 i = widths_size - 1; i > -1; --i) {
    width = widths[i];
    if (size::is_grow(width)) {
      new_rect.w = dynamic_width * size::decode_grow(width);
    } else {
      new_rect.w = size::decode_fixed(width);
    }

    rectangle.w -= new_rect.w;
    new_rect.x = rectangle.x + rectangle.w;
    error = this->state.widget_sizes.push(new_rect);
    if (error != error_codes::OK) {
      logger::fatal("Bad Allocation on widget_sizes");
      std::abort();
    }
  }
}

void Window::start_row(const ds::vector<i32>& widths) noexcept {
  this->start_row(widths.get_data(), widths.get_size());
}

void Window::end_row() noexcept {
  // TODO:
}

void Window::start_column(const i32* heights, i32 heights_size) noexcept {
  auto rectangle = this->state.widget_sizes.pop();

  i32 height = 0;
  f32 dynamic_height = rectangle.h;
  i32 parts = 0;
  for (i32 i = 0; i < heights_size; ++i) {
    height = heights[i];
    if (size::is_grow(height)) {
      parts += size::decode_grow(height);
    } else {
      dynamic_height -= size::decode_fixed(height);
    }
  }

  dynamic_height = std::max(dynamic_height, 0.0F);

  if (parts > 0) {
    dynamic_height /= parts;
  } else {
    rectangle.h -= dynamic_height;
  }

  rect<f32> new_rect{.x = rectangle.x, .w = rectangle.w};
  error_code error = error_codes::OK;
  for (i32 i = heights_size - 1; i > -1; --i) {
    height = heights[i];
    if (size::is_grow(height)) {
      new_rect.h = dynamic_height * size::decode_grow(height);
    } else {
      new_rect.h = size::decode_fixed(height);
    }

    rectangle.h -= new_rect.h;
    new_rect.y = rectangle.y + rectangle.h;
    error = this->state.widget_sizes.push(new_rect);
    if (error != error_codes::OK) {
      logger::fatal("Bad Allocation on widget_sizes");
      std::abort();
    }
  }
}

void Window::start_column(const ds::vector<i32>& heights) noexcept {
  this->start_column(heights.get_data(), heights.get_size());
}

void Window::end_column() noexcept {
  // TODO:
}

void Window::start_group() noexcept {
  // NOTE: Add a fatal so that stacking groups is not allowed
  this->state.limits = this->state.widget_sizes.pop();

  auto sdl_rect = SDL_Rect{
    .x = (i32)this->state.limits.x,
    .y = (i32)this->state.limits.y,
    .w = (i32)this->state.limits.w,
    .h = (i32)this->state.limits.h
  };
  SDL_SetRenderClipRect(this->renderer, &sdl_rect);
}

void Window::add_group(const rect<f32>& rectangle) noexcept {
  const rect<f32>& original = this->state.limits;
  auto error = this->state.widget_sizes.push(
      {.position = original.position + rectangle.position,
       .size = rectangle.size}
  );

  if (error != error_codes::OK) {
    logger::fatal("Bad Allocation on widget_sizes");
    std::abort();
  }
}

void Window::end_group() noexcept {
  SDL_SetRenderClipRect(this->renderer, nullptr);
}

// === Widgets === //

void Window::text(const c8* string) noexcept {
  const auto rectangle = this->state.widget_sizes.pop();
  i32 text_length = std::strlen(string);

  // Compute the rect
  vec2<i32> size{};
  TTF_GetStringSize(this->font, string, text_length, &size.x, &size.y);

  auto text_rect = calculate_widget_rectangle(rectangle, size.to<f32>(), this->state.limits.size);

  SDL_Texture* texture = create_text_texture(
      this->renderer, this->font, string, text_length,
      this->theme.foreground_color
  );
  SDL_RenderTexture(this->renderer, texture, nullptr, &text_rect);
  SDL_DestroyTexture(texture);
}

bool Window::text_button(const c8* text) noexcept {
  const auto rectangle = this->state.widget_sizes.pop();
  bool mouseover = rectangle.contains(this->input.mouse.position);
  i32 text_length = std::strlen(text);

  // Compute the rect
  vec2<i32> size{};
  TTF_GetStringSize(this->font, text, text_length, &size.x, &size.y);
  const auto text_rect = calculate_widget_rectangle(rectangle, size.to<f32>(), this->state.limits.size);

  // Draw button background
  const auto background_color =
      mouseover ? this->theme.foreground_color : this->theme.background_color;
  const auto foreground_color =
      mouseover ? this->theme.background_color : this->theme.foreground_color;
  set_color(this->renderer, background_color);
  SDL_RenderFillRect(this->renderer, (SDL_FRect*)&rectangle);
  set_color(this->renderer, foreground_color);
  SDL_RenderRect(this->renderer, (SDL_FRect*)&rectangle);

  SDL_Texture* texture = create_text_texture(
      this->renderer, this->font, text, text_length, foreground_color
  );
  SDL_RenderTexture(this->renderer, texture, nullptr, &text_rect);
  SDL_DestroyTexture(texture);

  return mouseover && this->input.mouse.left == MouseState::RELEASED;
}

void Window::rectangle(rgba8 color) noexcept {
  auto rectangle = this->state.widget_sizes.pop();
  if (immpp::size::is_type(rectangle.w)) {
    rectangle.w = this->state.limits.w;
  }
  if (immpp::size::is_type(rectangle.h)) {
    rectangle.h = this->state.limits.h;
  }

  set_color(this->renderer, color);
  SDL_RenderRect(this->renderer, (SDL_FRect*)&rectangle);
}

void Window::fill_rectangle(rgba8 color) noexcept {
  auto rectangle = this->state.widget_sizes.pop();
  if (immpp::size::is_type(rectangle.w)) {
    rectangle.w = this->state.limits.w;
  }
  if (immpp::size::is_type(rectangle.h)) {
    rectangle.h = this->state.limits.h;
  }

  set_color(this->renderer, color);
  SDL_RenderFillRect(this->renderer, (SDL_FRect*)&rectangle);
}

} // namespace immpp
